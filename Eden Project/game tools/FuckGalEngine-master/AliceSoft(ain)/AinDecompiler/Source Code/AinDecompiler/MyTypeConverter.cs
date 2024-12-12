using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.ComponentModel;
using System.Reflection;
using System.Diagnostics;

namespace AinDecompiler
{
    class MyPropertyDescriptorForFields : PropertyDescriptor
    {
        FieldInfo fieldInfo;
        Type componentType;
        MethodInfo shouldSerializeMethod;

        public static bool MemberIsBrowsable(MemberInfo memberInfo)
        {
            if (memberInfo.IsDefined(typeof(BrowsableAttribute), true))
            {
                var browsableAttribute = (BrowsableAttribute)memberInfo.GetCustomAttributes(typeof(BrowsableAttribute), true).FirstOrDefault();
                if (browsableAttribute != null && browsableAttribute.Browsable == false)
                {
                    return false;
                }
            }
            return true;
        }

        public static MyPropertyDescriptorForFields[] CreateArray(Type type, IEnumerable<Attribute> defaultCustomAttributes)
        {
            var fields = type.GetFields(BindingFlags.Instance | BindingFlags.Public | BindingFlags.FlattenHierarchy);
            var list = new List<MyPropertyDescriptorForFields>();
            foreach (var fieldInfo in fields)
            {
                //hide elements that aren't browsable
                if (MemberIsBrowsable(fieldInfo))
                {
                    list.Add(new MyPropertyDescriptorForFields(fieldInfo, defaultCustomAttributes));
                }
            }
            return list.ToArray();
        }

        static readonly HashSet<Type> NonExpandedTypes = new HashSet<Type>(new Type[] {
            typeof(sbyte), typeof(byte), typeof(short), typeof(ushort), typeof(int), typeof(uint), 
            typeof(long), typeof(ulong), typeof(char), typeof(float), typeof(double), typeof(bool), 
            typeof(decimal), typeof(string), typeof(DateTime)
        });

        public static Attribute[] GetAttributes(Type type, IEnumerable<Attribute> customAttributes)
        {
            List<Attribute> attributes = new List<Attribute>();
            attributes.AddRange(customAttributes);
            if (!NonExpandedTypes.Contains(type) &&
                !type.IsEnum)
            {
                bool neverExpandMember = false;
                bool showFieldsOfMember = true;
                bool makeExpandable = false;
                if (typeof(System.Collections.IEnumerable).IsAssignableFrom(type))
                {
                    showFieldsOfMember = false;
                    makeExpandable = true;
                }

                if (typeof(byte[]).IsAssignableFrom(type))
                {
                    neverExpandMember = true;
                }

                if (showFieldsOfMember)
                {
                    attributes.Add(new TypeConverterAttribute(typeof(MyTypeConverter)));
                }
                if (neverExpandMember)
                {
                    attributes.Add(new TypeConverterAttribute(typeof(ReferenceConverter)));
                    attributes.Add(new ReadOnlyAttribute(true));
                }
                if (makeExpandable && !neverExpandMember)
                {
                    attributes.Add(new TypeConverterAttribute(typeof(MyExpandableObjectConverter)));
                }
            }
            return attributes.ToArray();
        }

        private static Attribute[] GetAttributes(FieldInfo fieldInfo, IEnumerable<Attribute> defaultCustomAttributes)
        {
            var fieldType = fieldInfo.FieldType;
            var fieldAttributes = fieldInfo.GetCustomAttributes(true).OfType<Attribute>();

            return GetAttributes(fieldType, defaultCustomAttributes.Concat(fieldAttributes));
        }

        private static MethodInfo FindShouldSerializeMethod(FieldInfo fieldInfo)
        {
            var methods = fieldInfo.DeclaringType.GetMethods(BindingFlags.Public | BindingFlags.Instance | BindingFlags.FlattenHierarchy);
            string shouldSerializeName = "ShouldSerialize" + fieldInfo.Name;
            var method = methods.Where(m => m.Name == shouldSerializeName).FirstOrDefault();
            if (method == null)
            {
                return null;
            }
            if (method.ReturnType == typeof(bool) && method.GetParameters().Length == 0)
            {
                return method;
            }
            else
            {
                return null;
            }
        }

        public MyPropertyDescriptorForFields(FieldInfo fieldInfo, IEnumerable<Attribute> defaultCustomAttributes)
            : base(fieldInfo.Name, GetAttributes(fieldInfo, defaultCustomAttributes))
        {
            this.fieldInfo = fieldInfo;
            this.componentType = fieldInfo.DeclaringType;
            this.shouldSerializeMethod = FindShouldSerializeMethod(fieldInfo);
        }

        public override bool CanResetValue(object component)
        {
            DefaultValueAttribute defaultValueAttribute = (DefaultValueAttribute)this.Attributes[typeof(DefaultValueAttribute)];
            return defaultValueAttribute != null && defaultValueAttribute.Value.Equals(this.GetValue(component));
        }

        public override Type ComponentType
        {
            get
            {
                return this.componentType;
            }
        }

        public override object GetValue(object component)
        {
            return fieldInfo.GetValue(component);
        }

        public override TypeConverter Converter
        {
            get
            {
                return base.Converter;
            }
        }

        public override bool IsReadOnly
        {
            get
            {
                return this.Attributes.Contains(ReadOnlyAttribute.Yes) || fieldInfo.IsInitOnly;
            }
        }

        public override Type PropertyType
        {
            get
            {
                return fieldInfo.FieldType;
            }
        }

        public override void ResetValue(object component)
        {
            DefaultValueAttribute defaultValueAttribute = (DefaultValueAttribute)this.Attributes[typeof(DefaultValueAttribute)];
            if (defaultValueAttribute != null)
            {
                this.SetValue(component, defaultValueAttribute.Value);
            }
        }

        public override void SetValue(object component, object value)
        {
            fieldInfo.SetValue(component, value);
        }

        public override bool ShouldSerializeValue(object component)
        {
            if (shouldSerializeMethod == null)
            {
                return true;
            }
            return (bool)shouldSerializeMethod.Invoke(component, null);
        }
    }

    class MyPropertyDescriptorForProperties : PropertyDescriptor
    {
        PropertyInfo propertyInfo;
        Type componentType;
        MethodInfo shouldSerializeMethod;

        public static bool MemberIsBrowsable(MemberInfo memberInfo)
        {
            if (memberInfo.IsDefined(typeof(BrowsableAttribute), true))
            {
                var browsableAttribute = (BrowsableAttribute)memberInfo.GetCustomAttributes(typeof(BrowsableAttribute), true).FirstOrDefault();
                if (browsableAttribute != null && browsableAttribute.Browsable == false)
                {
                    return false;
                }
            }
            return true;
        }

        public static MyPropertyDescriptorForProperties[] CreateArray(Type type, IEnumerable<Attribute> defaultCustomAttributes)
        {
            var properties = type.GetProperties(BindingFlags.Instance | BindingFlags.Public | BindingFlags.FlattenHierarchy);
            var list = new List<MyPropertyDescriptorForProperties>();
            foreach (var propertyInfo in properties)
            {
                if (MemberIsBrowsable(propertyInfo))
                {
                    list.Add(new MyPropertyDescriptorForProperties(propertyInfo, defaultCustomAttributes));
                }
            }
            return list.ToArray();
        }

        static readonly HashSet<Type> NonExpandedTypes = new HashSet<Type>(new Type[] 
        {
            typeof(sbyte), typeof(byte), typeof(short), typeof(ushort), typeof(int), typeof(uint), 
            typeof(long), typeof(ulong), typeof(char), typeof(float), typeof(double), typeof(bool), 
            typeof(decimal), typeof(string), typeof(DateTime) 
        });

        private static Attribute[] GetAttributes(PropertyInfo propertyInfo, IEnumerable<Attribute> defaultCustomAttributes)
        {
            var propertyAttributes = propertyInfo.GetCustomAttributes(true).OfType<Attribute>();

            var attributes = MyPropertyDescriptorForFields.GetAttributes(propertyInfo.PropertyType, propertyAttributes.Concat(defaultCustomAttributes));

            //var attributes = new List<Attribute>();
            //attributes.AddRange(defaultCustomAttributes);
            //var customAttributes = propertyInfo.GetCustomAttributes(true);
            //attributes.AddRange(customAttributes.OfType<Attribute>());
            ////var attributes = customAttributes.OfType<Attribute>().ToList();
            //if (!NonExpandedTypes.Contains(propertyInfo.PropertyType) &&
            //    !propertyInfo.PropertyType.IsEnum)
            //{
            //    bool implementsIEnumerable = typeof(System.Collections.IEnumerable).IsAssignableFrom(propertyInfo.PropertyType);
            //    if (!implementsIEnumerable)
            //    {
            //        attributes.Add(new TypeConverterAttribute(typeof(MyTypeConverter)));
            //    }
            //    attributes.AddRange(propertyInfo.GetCustomAttributes(true).OfType<Attribute>());
            //}
            return attributes;
        }


        private static MethodInfo FindShouldSerializeMethod(PropertyInfo propertyInfo)
        {
            var methods = propertyInfo.DeclaringType.GetMethods(BindingFlags.Public | BindingFlags.Instance | BindingFlags.FlattenHierarchy);
            string shouldSerializeName = "ShouldSerialize" + propertyInfo.Name;
            var method = methods.Where(m => m.Name == shouldSerializeName).FirstOrDefault();
            if (method == null)
            {
                return null;
            }
            if (method.ReturnType == typeof(bool) && method.GetParameters().Length == 0)
            {
                return method;
            }
            else
            {
                return null;
            }
        }

        public MyPropertyDescriptorForProperties(PropertyInfo propertyInfo, IEnumerable<Attribute> defaultCustomAttributes)
            : base(propertyInfo.Name, GetAttributes(propertyInfo, defaultCustomAttributes))
        {
            this.propertyInfo = propertyInfo;
            this.componentType = propertyInfo.DeclaringType;
            this.shouldSerializeMethod = FindShouldSerializeMethod(propertyInfo);
        }

        public override bool CanResetValue(object component)
        {
            DefaultValueAttribute defaultValueAttribute = (DefaultValueAttribute)this.Attributes[typeof(DefaultValueAttribute)];
            return defaultValueAttribute != null && defaultValueAttribute.Value.Equals(this.GetValue(component));
        }

        public override Type ComponentType
        {
            get
            {
                return this.componentType;
            }
        }

        public override object GetValue(object component)
        {
            if (propertyInfo.GetIndexParameters().Length == 0)
            {
                try
                {
                    return propertyInfo.GetValue(component, null);
                }
                catch (Exception)
                {
                    return component;
                }
            }
            return component;
        }

        public override bool IsReadOnly
        {
            get
            {
                return this.Attributes.Contains(ReadOnlyAttribute.Yes) || !propertyInfo.CanWrite;
            }
        }

        public override Type PropertyType
        {
            get
            {
                return propertyInfo.PropertyType;
            }
        }

        public override void ResetValue(object component)
        {
            DefaultValueAttribute defaultValueAttribute = (DefaultValueAttribute)this.Attributes[typeof(DefaultValueAttribute)];
            if (defaultValueAttribute != null)
            {
                this.SetValue(component, defaultValueAttribute.Value);
            }
        }

        public override void SetValue(object component, object value)
        {
            propertyInfo.SetValue(component, value, null);
        }

        public override bool ShouldSerializeValue(object component)
        {
            if (this.shouldSerializeMethod == null)
            {
                return true;
            }
            else
            {
                return (bool)this.shouldSerializeMethod.Invoke(component, null);
            }
        }
    }

    //class MyPropertyDescriptorForCollections : PropertyDescriptor
    //{
    //    Type myType;
    //    int index;

    //    public override bool CanResetValue(object component)
    //    {
    //        return false;
    //    }

    //    public override Type ComponentType
    //    {
    //        get 
    //        {
    //            return myType;
    //        }
    //    }

    //    public override object GetValue(object component)
    //    {
    //        throw new NotImplementedException();
    //    }

    //    public override bool IsReadOnly
    //    {
    //        get { throw new NotImplementedException(); }
    //    }

    //    public override Type PropertyType
    //    {
    //        get { throw new NotImplementedException(); }
    //    }

    //    public override void ResetValue(object component)
    //    {
    //        throw new NotImplementedException();
    //    }

    //    public override void SetValue(object component, object value)
    //    {
    //        throw new NotImplementedException();
    //    }

    //    public override bool ShouldSerializeValue(object component)
    //    {
    //        throw new NotImplementedException();
    //    }
    //}


    class MyTypeConverter : ExpandableObjectConverter
    {
        public override PropertyDescriptorCollection GetProperties(ITypeDescriptorContext context, object value, Attribute[] attributes)
        {
            if (value.GetType().Namespace == "System.ComponentModel.Design")
            {
                return TypeDescriptor.GetProperties(value);

                //var type = value.GetType();
                //var valueField = type.GetField("value", BindingFlags.NonPublic | BindingFlags.Instance);
                //if (valueField != null)
                //{
                //    value = valueField.GetValue(value);
                //}
            }
            var arr = MyPropertyDescriptorForFields.CreateArray(value.GetType(), attributes);
            var arr2 = MyPropertyDescriptorForProperties.CreateArray(value.GetType(), attributes);
            return new PropertyDescriptorCollection(arr.Concat<PropertyDescriptor>(arr2).ToArray());
        }

        //public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType)
        //{
        //    if (destinationType == context.Instance.GetType())
        //    {
        //        return true;
        //    }
        //    return base.CanConvertTo(context, destinationType);
        //}
    }

    class MyArrayPropertyDescriptor<T> : PropertyDescriptor
    {
        IList<T> array;
        int index;

        static string GetPropertyName(IList<T> array, int index)
        {
            if (index >= array.Count)
            {
                return "<new item>";
            }
            return "[" + index.ToString() + "]";
        }

        static Attribute[] GetAttributes(IList<T> array, int index)
        {
            return MyPropertyDescriptorForFields.GetAttributes(typeof(T), new Attribute[0]);
        }

        public MyArrayPropertyDescriptor(IList<T> array, int index)
            : base(GetPropertyName(array, index), GetAttributes(array, index))
        {
            this.array = array;
            this.index = index;
        }

        public override bool CanResetValue(object component)
        {
            return false;
        }

        public override Type ComponentType
        {
            get
            {
                return this.array.GetType();
            }
        }

        public override object GetValue(object component)
        {
            if (index >= array.Count || index < 0)
            {
                return null;
            }
            else
            {
                return array[index];
            }
        }

        public override bool IsReadOnly
        {
            get
            {
                return false;
            }
        }

        public override Type PropertyType
        {
            get
            {
                return typeof(T);
            }
        }

        public override void ResetValue(object component)
        {

        }

        public override void SetValue(object component, object objValue)
        {
            if (objValue != null && objValue is T)
            {
                var value = (T)objValue;
                if (this.index == this.array.Count)
                {
                    try
                    {
                        this.array.Add(value);
                    }
                    catch (InvalidOperationException)
                    {

                    }
                }
                else if (this.index >= 0 && this.index < array.Count)
                {
                    this.array[this.index] = value;
                }
            }
        }

        public override bool ShouldSerializeValue(object component)
        {
            return true;
        }
    }

    class MyExpandableObjectConverter : ExpandableObjectConverter
    {
        public override PropertyDescriptorCollection GetProperties(ITypeDescriptorContext context, object value, Attribute[] attributes)
        {
            if (value.GetType().Namespace == "System.ComponentModel.Design")
            {
                return TypeDescriptor.GetProperties(value);
            }

            var type = value.GetType();
            var interfaces = type.GetInterfaces();

            string ilistNamespace = typeof(IList<int>).Namespace; //System.Collections.Generic
            string ilistName = typeof(IList<int>).Name; //IList`1

            List<PropertyDescriptor> propertyDescriptors = new List<PropertyDescriptor>();

            foreach (var interfaceInfo in interfaces)
            {
                if (interfaceInfo.Namespace == ilistNamespace && interfaceInfo.Name == ilistName)
                {
                    var list = (System.Collections.IList)value;
                    var memberType = interfaceInfo.GetGenericArguments().FirstOrDefault();
                    var dummyType = typeof(MyArrayPropertyDescriptor<int>);
                    var propertyDescriptorType = Type.GetType(dummyType.Namespace + "." + dummyType.Name + "[[" + memberType.FullName + "]]");

                    for (int i = 0; i < list.Count; i++)
                    {
                        var propertyDescriptor = (PropertyDescriptor)Activator.CreateInstance(propertyDescriptorType, value, i);
                        propertyDescriptors.Add(propertyDescriptor);
                    }
                }
            }

            return new PropertyDescriptorCollection(propertyDescriptors.ToArray());
        }

        public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destinationType)
        {
            if (destinationType == typeof(string) && value != null)
            {
                var collection = value as System.Collections.ICollection;
                if (collection != null)
                {
                    return "Count = " + collection.Count;
                }
            }
            var defaultResult = base.ConvertTo(context, culture, value, destinationType);
            return defaultResult;
        }
    }

    //class MyReferenceConverter : ReferenceConverter
    //{
    //    public MyReferenceConverter(Type type)
    //        : base(type)
    //    {

    //    }

    //    public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destinationType)
    //    {
    //        return base.ConvertTo(context, culture, value, destinationType);
    //    }
    //}

    public class PropertyGridWrapper
    {
        [TypeConverter(typeof(MyTypeConverter))]
        public object Object { get; set; }
    }
}
