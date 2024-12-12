using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler
{
    public class HistogramEntry<T> : IHistogramEntry
    {
        public int Count;
        public T Value;
        public bool IsNull;
        public double Rate;

        #region IHistogramEntry Members

        int IHistogramEntry.Count
        {
            get
            {
                return Count;
            }
        }

        double IHistogramEntry.Rate
        {
            get
            {
                return Rate;
            }
        }

        bool IHistogramEntry.IsNull
        {
            get
            {
                return IsNull;
            }
        }

        int IHistogramEntry.IntValue
        {
            get
            {
                if (typeof(T) == typeof(int))
                {
                    return (int)(object)Value;
                }
                else
                {
                    return 0;
                }
            }
        }

        string IHistogramEntry.StringValue
        {
            get
            {
                if (typeof(T) == typeof(string))
                {
                    return (string)(object)Value;
                }
                else
                {
                    return null;
                }
            }
        }

        float IHistogramEntry.FloatValue
        {
            get
            {
                if (typeof(T) == typeof(float))
                {
                    return (float)(object)Value;
                }
                else
                {
                    return 0;
                }
            }
        }

        #endregion

        public string GetValueString()
        {
            if (this.IsNull)
            {
                return "(null)";
            }
            if (typeof(T) == typeof(float))
            {
                return ((float)(object)Value).FloatToString();
            }
            else if (typeof(T) == typeof(string))
            {
                return ((string)(object)Value);
            }
            else if (typeof(T) == typeof(int))
            {
                return ((int)(object)Value).ToString();
            }
            else
            {
                return "";
            }
        }

        public InitialValue ToInitialValue()
        {
            if (this.IsNull)
            {
                return null;
            }
            if (typeof(T) == typeof(float))
            {
                return new InitialValue((float)(object)Value);
            }
            else if (typeof(T) == typeof(string))
            {
                return new InitialValue((string)(object)Value);
            }
            else if (typeof(T) == typeof(int))
            {
                return new InitialValue((int)(object)Value);
            }
            else
            {
                return null;
            }

        }

    }

    /// <summary>
    /// Cast this to HistogramEntry&lt;int&gt;, HistogramEntry&lt;float&gt;, or HistogramEntry&lt;string&gt;.
    /// </summary>
    public interface IHistogramEntry
    {
        // Cast this to HistogramEntry<int>, HistogramEntry<float>, or HistogramEntry<string>.
        int Count { get; }
        int IntValue { get; }
        string StringValue { get; }
        float FloatValue { get; }
        double Rate { get; }
        bool IsNull { get; }
        string GetValueString();
        InitialValue ToInitialValue();
    }

    public class Histogram<T>
    {
        Dictionary<T, int> counts = new Dictionary<T, int>();
        int nullCount = 0;
        public void Add(T value)
        {
            if (counts.ContainsKey(value))
            {
                counts[value]++;
            }
            else
            {
                counts.Add(value, 1);
            }
        }
        public void AddNull()
        {
            nullCount++;
        }
        public HistogramEntry<T>[] GetResults()
        {
            int total = nullCount + counts.Values.Sum();
            return counts.Select(pair =>
                new HistogramEntry<T>()
                {
                    Count = pair.Value,
                    Value = pair.Key,
                    Rate = (double)pair.Value / (double)total
                })
                .Concat(Enumerable.Repeat(new HistogramEntry<T>()
                {
                    Count = nullCount,
                    IsNull = true,
                    Rate = (double)nullCount / (double)total
                }, nullCount > 0 ? 1 : 0))
                .OrderByDescending(entry => entry.Count).ToArray();
        }
    }

    //class DefaultArgumentResult
    //{
    //    public static DefaultArgumentResult[] Create(HistogramEntry<int>[] entries)
    //    {
    //        return entries.Select(entry =>
    //            new DefaultArgumentResult()
    //            {
    //                Count = entry.Count,
    //                IntValue = entry.Value,
    //                Rate = entry.Rate,
    //                DataType = entry.IsNull ? DataType.Void : DataType.Int
    //            }).ToArray();
    //    }
    //    public static DefaultArgumentResult[] Create(HistogramEntry<float>[] entries)
    //    {
    //        return entries.Select(entry =>
    //            new DefaultArgumentResult()
    //            {
    //                Count = entry.Count,
    //                FloatValue = entry.Value,
    //                Rate = entry.Rate,
    //                DataType = entry.IsNull ? DataType.Void : DataType.Float
    //            }).ToArray();
    //    }
    //    public static DefaultArgumentResult[] Create(HistogramEntry<string>[] entries)
    //    {
    //        return entries.Select(entry =>
    //            new DefaultArgumentResult()
    //            {
    //                Count = entry.Count,
    //                StringValue = entry.Value,
    //                Rate = entry.Rate,
    //                DataType = entry.IsNull ? DataType.Void : DataType.String
    //            }).ToArray();
    //    }

    //    public int Count;
    //    public DataType DataType;
    //    public int IntValue;
    //    public float FloatValue;
    //    public string StringValue;
    //    public double Rate;
    //    public bool HasValue
    //    {
    //        get
    //        {
    //            return DataType != DataType.Void;
    //        }
    //    }
    //    public bool IsInt
    //    {
    //        get
    //        {
    //            return DataType == DataType.Int || DataType == DataType.RefInt || DataType == DataType.Lint || DataType == DataType.RefLint;
    //        }
    //    }
    //    public bool IsFloat
    //    {
    //        get
    //        {
    //            return DataType == DataType.Float || DataType == DataType.RefFloat;
    //        }
    //    }
    //    public bool IsString
    //    {
    //        get
    //        {
    //            return DataType == DataType.String || DataType == DataType.RefString;
    //        }
    //    }
    //    public bool IsFunctype
    //    {
    //        get
    //        {
    //            return DataType == DataType.Functype || DataType == DataType.RefFunctype;
    //        }
    //    }
    //}

    class DefaultArgumentFinder
    {
        AinFile ainFile;
        public DefaultArgumentFinder(AinFile ainFile)
        {
            this.ainFile = ainFile;
        }

        private static IHistogramEntry[] _blankArray = new IHistogramEntry[0];
        public IHistogramEntry[] FindArgumentValues(Variable argument)
        {
            switch (argument.DataType)
            {
                case DataType.Int:
                case DataType.Lint:
                case DataType.RefInt:
                case DataType.RefLint:
                    return FindArgumentValuesInt(argument);
                case DataType.Float:
                case DataType.RefFloat:
                    return FindArgumentValuesFloat(argument);
                case DataType.String:
                case DataType.RefString:
                    return FindArgumentValuesString(argument);
            }
            return _blankArray;
        }

        private HistogramEntry<T>[] FindArgumentValues<T>(Variable argument, Instruction expressionType, Func<Expression, T> GetValue)
        {
            Function functionToFind = argument.Parent as Function;
            var functionsUsing = ainFile.CodeSearchCache.FunctionsCache.GetUsedBy(functionToFind);
            Histogram<T> histogram = new Histogram<T>();
            foreach (var function in functionsUsing)
            {
                var expressionMap = ainFile.DecompiledCodeCache.GetExpressionMap(function);
                foreach (var node in expressionMap.FindVariable(functionToFind))
                {
                    var expression = node.item;
                    var argumentExpression = expression.GetFunctionCallArgument(argument.Index);

                    if (argumentExpression != null)
                    {
                        argumentExpression = argumentExpression.SkipChildCastOperations();
                        if (argumentExpression.ExpressionType == expressionType)
                        {
                            var value = GetValue(argumentExpression);
                            if (value != null)
                            {
                                histogram.Add(value);
                            }
                            else
                            {
                                histogram.AddNull();
                            }
                        }
                        else
                        {
                            histogram.AddNull();
                        }
                    }
                }
            }
            return histogram.GetResults();
        }

        public HistogramEntry<int>[] FindArgumentValuesInt(Variable argument)
        {
            return FindArgumentValues(argument, Instruction.PUSH, expression => expression.Value);
        }
        public HistogramEntry<float>[] FindArgumentValuesFloat(Variable argument)
        {
            return FindArgumentValues(argument, Instruction.F_PUSH, expression =>
            {
                var bytes = BitConverter.GetBytes(expression.Value);
                float floatValue = BitConverter.ToSingle(bytes, 0);
                return floatValue;
            });
        }
        public HistogramEntry<string>[] FindArgumentValuesString(Variable argument)
        {
            return FindArgumentValues(argument, Instruction.S_PUSH, expression => ainFile.GetString(expression.Value));
        }

        private Expression GetFunctionArgument(Expression functionCallExpression, int argumentNumber)
        {
            if (functionCallExpression.Parent == null) return null;

            var expression = functionCallExpression.Arg1;
            if (expression == null) return null;

            int currentArgumnetNumber = argumentNumber;

            if (functionCallExpression.ExpressionType == Instruction.CALLMETHOD)
            {
                currentArgumnetNumber++;
            }

            while (currentArgumnetNumber > 0)
            {
                if (expression.ExpressionType == Instruction.Comma)
                {
                    expression = expression.Arg2;
                    if (expression == null) return null;
                }
                else
                {
                    break;
                }
                currentArgumnetNumber--;
            }
            if (expression.ExpressionType == Instruction.Comma)
            {
                expression = expression.Arg1;
                if (expression == null) return null;
            }
            while (ExpressionDisplayer.castOperations.Contains(expression.ExpressionType))
            {
                expression = expression.Arg1;
                if (expression == null) return null;
            }
            return expression;
        }
    }
}
