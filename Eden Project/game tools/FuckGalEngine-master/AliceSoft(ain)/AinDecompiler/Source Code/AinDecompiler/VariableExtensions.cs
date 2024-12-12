using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler
{
    public static partial class VariableExtensions
    {
        /// <summary>
        /// Checks if a variable object is a reference to a variable registered in the AinFile, and returns the original variable from the AinFile if it exists, otherwise returns null.
        /// </summary>
        /// <param name="variable">The variable to find</param>
        /// <returns>returns the original variable from the AinFile if it exists, otherwise returns null</returns>
        public static IVariable Canonicalize(this IVariable variable)
        {
            if (variable == null)
            {
                return null;
            }
            return Canonicalize(variable, variable.Root);
        }

        /// <summary>
        /// Checks if a variable object is a reference to a variable registered in the AinFile, and returns the original variable from the AinFile if it exists, otherwise returns null.
        /// </summary>
        /// <param name="variable">The variable to find</param>
        /// <param name="ainFile">The AinFile to find the variable in.</param>
        /// <returns>returns the original variable from the AinFile if it exists, otherwise returns null</returns>
        public static IVariable Canonicalize(this IVariable variable, AinFile ainFile)
        {
            int index = variable.Index;
            int parentIndex = -1;
            if (variable == null || ainFile == null)
            {
                return null;
            }
            var parent = variable.Parent;
            if (parent != null)
            {
                parentIndex = parent.Index;
            }

            if (parent is Function)
            {
                if (parent.Name.StartsWith("system."))
                {
                    return AinFile.GetSystemCallParameter(parentIndex, index);
                }
                return ainFile.GetFunctionParameter(parentIndex, index);
            }
            if (parent is Struct)
            {
                return ainFile.GetStructMember(parentIndex, index);
            }
            if (parent is FunctionType)
            {
                return ainFile.GetFuncTypeParameter(parentIndex, index);
            }
            if (parent is HllFunction)
            {
                if (parent.Parent != null)
                {
                    return ainFile.GetLibraryFunctionParameter(parent.Parent.Index, parentIndex, index);
                }
                else
                {
                    return null;
                }
            }
            if (variable is Function)
            {
                if (variable.Name.StartsWith("system."))
                {
                    return AinFile.GetSystemCall(index);
                }
                return ainFile.GetFunction(index);
            }
            if (variable is Struct)
            {
                return ainFile.GetStruct(index);
            }
            if (variable is Global)
            {
                return ainFile.GetGlobal(index);
            }
            if (variable is HllFunction)
            {
                return ainFile.GetLibraryFunction(parentIndex, index);
            }
            if (variable is FunctionType)
            {
                return ainFile.GetFuncType(index);
            }
            return null;
        }

        /// <summary>
        /// Returns the MetaData object associated with this variable, returns null if it doesn't exist.
        /// </summary>
        /// <param name="variable">The variable to find the metadata object for</param>
        /// <returns>The MetaData object associated with this variable, or null</returns>
        public static Metadata GetMetadata(this IVariable variable)
        {
            var ainFile = variable.Root;
            if (ainFile == null) return null;
            return ainFile.MetadataFile.Metadata.GetOrNull(variable);
        }

        /// <summary>
        /// Returns the alternative name for this variable from metadata, or from the AlternativeNames dictionary.  If none is found, returns the original variable name.
        /// </summary>
        /// <param name="variable">The variable to find the metadata object for</param>
        /// <returns>The alternative name from metadata, the AlternativeNames dictionary, or the original name</returns>
        public static string GetAlternativeName(this IVariable variable)
        {
            var ainFile = variable.Root;
            string variableName = variable.Name;
            var metaData = variable.GetMetadata();
            if (metaData != null && metaData.ReplacementName != null)
            {
                return metaData.ReplacementName;
            }
            else if (ainFile != null && ainFile.AlternativeNames.ContainsKey(variableName))
            {
                return ainFile.AlternativeNames[variableName];
            }
            return variableName;
        }

        /// <summary>
        /// Returns the alternative global group name for a global variable.
        /// </summary>
        /// <param name="global">The global variable to find the global group metadata for</param>
        /// <returns>The alternative name from metadata, the AlternativeNames dictionary, or the original name</returns>
        public static string GetAlternativeGlobalGroupName(this Global global)
        {
            var ainFile = global.Root;
            string variableName = ainFile.GlobalGroupNames.GetOrDefault(global.GroupIndex, "");
            var metadata = ainFile.MetadataFile.GlobalGroupMetadata.GetOrNull(global.GroupIndex);
            if (metadata != null && metadata.ReplacementName != null)
            {
                return metadata.ReplacementName;
            }
            else if (ainFile != null && ainFile.AlternativeNames.ContainsKey(variableName))
            {
                return ainFile.AlternativeNames[variableName];
            }
            return variableName;
        }

        /// <summary>
        /// Returns the alternative name of the variable if DisplayAlternativeNames is true, otherwise returns the original variable name.
        /// </summary>
        /// <param name="variable"></param>
        /// <returns></returns>
        public static string GetActiveName(this IVariable variable)
        {
            ////DEBUG CODE!  DELETEME
            //var frames = new System.Diagnostics.StackTrace().GetFrames();
            //var toStringMethod = frames.Select(f => f.GetMethod()).Where(m => m.DeclaringType == typeof(Function) && m.Name == "ToString").FirstOrDefault();
            //if (toStringMethod == null)
            //{
            //}

            var ainFile = variable.Root;
            string variableName = variable.Name;
            if (ainFile != null && !ainFile.DisplayAlternativeNames)
            {
                return variableName;
            }
            else
            {
                return variable.GetAlternativeName();
            }
        }

        public static IVariable GetNonVoidFunctionParameter(this IFunction function, int variableIndex)
        {
            int count = variableIndex;
            int i = 0;
            while (count >= 0)
            {
                if (i >= function.ParameterCount) return null;
                var variable = function.Parameters[i];
                if (variable.DataType == DataType.Void)
                {
                    i++;
                }
                if (count == 0)
                {
                    break;
                }
                i++;
                count--;
            }
            return function.Parameters.GetOrNull(i);
        }

        public static InitialValue GetDefaultValue(this IVariable variable)
        {
            var metaData = variable.GetMetadata();
            if (metaData != null)
            {
                return metaData.DefaultValue;
            }
            return null;
        }

        public static bool HasDefaultValue(this IVariable variable)
        {
            return GetDefaultValue(variable) != null;
        }


        //public static Variable FindVariable(this Function function, string variableName)
        //{
        //    for (int i = 0; i < function.Parameters.Length; i++)
        //    {
        //        var parameter = function.Parameters[i];
        //        if (parameter.Name == variableName)
        //        {
        //            return parameter;
        //        }
        //    }
        //    return null;
        //}
    }
}
