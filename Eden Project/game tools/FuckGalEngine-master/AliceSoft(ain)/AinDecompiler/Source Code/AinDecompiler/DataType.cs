using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler
{
    public enum DataType
    {
        Void = 0,
        Int = 10,
        Float = 11,
        String = 12,
        Struct = 13,
        ArrayInt = 14,
        ArrayFloat = 15,
        ArrayString = 16,
        ArrayStruct = 17,
        RefInt = 18,
        RefFloat = 19,
        RefString = 20,
        RefStruct = 21,
        RefArrayInt = 22,
        RefArrayFloat = 23,
        RefArrayString = 24,
        RefArrayStruct = 25,
        IMainSystem = 26,
        Functype = 27,
        ArrayFunctype = 30,
        RefFunctype = 31,
        RefArrayFunctype = 32,
        Bool = 47,
        ArrayBool = 50,
        RefBool = 51,
        RefArrayBool = 52,
        Lint = 55,
        ArrayLint = 58,  //guess
        RefLint = 59, //guess
        RefArrayLint = 60,
        Delegate = 63,
        ArrayDelegate = 66, //guess
        RefDelegate = 67, //guess
        RefArrayDelegate = 69, //guess

        //fake types
        LocalPage = -1,
        GlobalPage = -2,
        LocalStructPage = -3,
    }

    public enum OldDataTypes
    {
        Imainsystem = 26,

        Function = 27,
        ArrayFunction = 30,
        RefFunction = 31,
        RefArrayFunction = 32,

        Long = 55,
        ArrayLong = 58,  //guess
        RefLong = 59, //guess
        RefArrayLong = 60,
    }
}
