using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler
{
    public static class DataTypeExtensions
    {
        public static bool IsRef(this DataType dataType)
        {
            return (dataType >= DataType.RefInt && dataType <= DataType.RefArrayStruct) ||
                dataType == DataType.RefBool || dataType == DataType.RefFunctype || dataType == DataType.RefLint ||
                dataType == DataType.RefArrayLint || dataType == DataType.RefArrayFunctype || dataType == DataType.RefArrayBool ||
                dataType == DataType.RefDelegate;
        }

        public static bool IsArray(this DataType dataType)
        {
            return (dataType >= DataType.ArrayInt && dataType <= DataType.ArrayStruct) ||
                (dataType >= DataType.RefArrayInt && dataType <= DataType.RefArrayStruct) ||
                dataType == DataType.ArrayBool || dataType == DataType.RefArrayBool ||
                dataType == DataType.ArrayLint || dataType == DataType.RefArrayLint ||
                dataType == DataType.ArrayFunctype || dataType == DataType.RefArrayFunctype ||
                dataType == DataType.ArrayDelegate || dataType == DataType.RefArrayDelegate;
        }

        public static bool IsRefArray(this DataType dataType)
        {
            return (dataType >= DataType.RefArrayInt && dataType <= DataType.RefArrayStruct) ||
                dataType == DataType.RefArrayLint || dataType == DataType.RefArrayBool || dataType == DataType.RefArrayFunctype ||
                dataType == DataType.RefArrayDelegate;
        }

        public static DataType GetTypeOfArrayElement(this DataType dataType)
        {
            if (dataType >= DataType.ArrayInt && dataType <= DataType.ArrayStruct)
            {
                return dataType - 4;
            }
            if (dataType >= DataType.RefArrayInt && dataType <= DataType.RefArrayStruct)
            {
                return dataType - 12;
            }
            if (dataType == DataType.ArrayBool || dataType == DataType.RefArrayBool)
            {
                return DataType.Bool;
            }
            if (dataType == DataType.ArrayLint || dataType == DataType.RefArrayLint)
            {
                return DataType.Lint;
            }
            if (dataType == DataType.ArrayFunctype || dataType == DataType.RefArrayFunctype)
            {
                return DataType.Functype;
            }
            if (dataType == DataType.ArrayDelegate || dataType == DataType.RefArrayDelegate)
            {
                return DataType.Delegate;
            }
            return dataType;
        }

        public static DataType GetTypeOfRef(this DataType dataType)
        {
            if (dataType >= DataType.RefInt && dataType <= DataType.RefStruct)
            {
                return dataType - 8;
            }
            if (dataType >= DataType.RefArrayInt && dataType <= DataType.RefArrayStruct)
            {
                return dataType - 8;
            }
            if (dataType == DataType.RefBool)
            {
                return DataType.Bool;
            }
            if (dataType == DataType.RefFunctype)
            {
                return DataType.Functype;
            }
            if (dataType == DataType.RefLint)
            {
                return DataType.Lint;
            }
            if (dataType == DataType.RefDelegate)
            {
                return DataType.Delegate;
            }
            if (dataType == DataType.RefArrayLint)
            {
                return DataType.ArrayLint;
            }
            if (dataType == DataType.RefArrayBool)
            {
                return DataType.ArrayBool;
            }
            if (dataType == DataType.RefArrayFunctype)
            {
                return DataType.ArrayFunctype;
            }
            if (dataType == DataType.RefArrayDelegate)
            {
                return DataType.ArrayDelegate;
            }
            return dataType;
        }

        public static bool IsInteger(this DataType dataType)
        {
            switch (dataType)
            {
                case DataType.Int:
                case DataType.Lint:
                case DataType.RefInt:
                case DataType.RefLint:
                    return true;
            }
            return false;
        }

        public static bool IsFloat(this DataType dataType)
        {
            return dataType == DataType.Float || dataType == DataType.RefFloat;
        }

        public static bool IsString(this DataType dataType)
        {
            return dataType == DataType.String || dataType == DataType.RefString;
        }

        public static bool IsFunction(this DataType dataType)
        {
            return dataType == DataType.Functype ||
                dataType == DataType.RefFunctype ||
                dataType == DataType.ArrayFunctype ||
                dataType == DataType.RefArrayFunctype ||
                dataType == DataType.Delegate ||
                dataType == DataType.RefDelegate ||
                dataType == DataType.ArrayDelegate ||
                dataType == DataType.RefArrayDelegate;
        }

        public static bool IsPrimitiveRefType(this DataType dataType)
        {
            return dataType == DataType.RefInt ||
                dataType == DataType.RefFloat ||
                dataType == DataType.RefBool ||
                dataType == DataType.RefLint;
        }
    }
}
