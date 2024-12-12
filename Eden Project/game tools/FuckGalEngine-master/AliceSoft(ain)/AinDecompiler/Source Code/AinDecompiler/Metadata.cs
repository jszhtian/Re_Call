using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler
{
    public class Metadata : ICloneable
    {
        public string ReplacementName;
        public string Description;
        public EnumerationType EnumerationType;
        public EnumerationType EnumerationTypeForArrayIndex;
        public InitialValue DefaultValue;

        public Metadata Clone()
        {
            var clone = (Metadata)this.MemberwiseClone();
            if (clone.EnumerationType != null)
            {
                clone.EnumerationType = this.EnumerationType.Clone();
            }
            if (clone.EnumerationTypeForArrayIndex != null)
            {
                clone.EnumerationTypeForArrayIndex = this.EnumerationTypeForArrayIndex.Clone();
            }
            if (clone.DefaultValue != null)
            {
                clone.DefaultValue = this.DefaultValue.Clone();
            }
            return clone;
        }

        #region ICloneable Members

        object ICloneable.Clone()
        {
            return Clone();
        }

        #endregion
    }

    public class EnumerationType : Dictionary<int, string>, ICloneable
    {
        public string Name;

        public EnumerationType Clone()
        {
            var newEnumerationType = new EnumerationType();
            newEnumerationType.AddRange(this);
            newEnumerationType.Name = this.Name;
            return newEnumerationType;
        }

        #region ICloneable Members

        object ICloneable.Clone()
        {
            return Clone();
        }

        #endregion
    }
}
