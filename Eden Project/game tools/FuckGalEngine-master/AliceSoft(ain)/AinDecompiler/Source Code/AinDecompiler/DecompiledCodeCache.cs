using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler
{
    public class DecompiledCodeCache
    {
        AinFile ainFile;
        Decompiler decompiler;
        public DictionaryWrapper<int, Expression> ExpressionItems;
        public DictionaryWrapper<int, string> CodeTextItems;
        public DictionaryWrapper<int, ExpressionMap> ExpressionMapItems;
        Dictionary<int, Expression> expressionCache = new Dictionary<int, Expression>();
        Dictionary<int, string> codeTextCache = new Dictionary<int, string>();
        Dictionary<int, ExpressionMap> expressionMapCache = new Dictionary<int, ExpressionMap>();

        private DecompiledCodeCache()
        {
            this.ExpressionItems = new DictionaryWrapper<int, Expression>(expressionCache);
            this.CodeTextItems = new DictionaryWrapper<int, string>(codeTextCache);
            this.ExpressionMapItems = new DictionaryWrapper<int, ExpressionMap>(expressionMapCache);
        }

        public DecompiledCodeCache(AinFile ainFile)
            : this()
        {
            this.ainFile = ainFile;
            this.decompiler = new Decompiler(ainFile);
        }

        public void Invalidate(int functionAddress)
        {
            if (expressionCache.ContainsKey(functionAddress))
            {
                expressionCache.Remove(functionAddress);
            }
            if (codeTextCache.ContainsKey(functionAddress))
            {
                codeTextCache.Remove(functionAddress);
            }
            if (expressionMapCache.ContainsKey(functionAddress))
            {
                expressionMapCache.Remove(functionAddress);
            }
        }

        public void Invalidate(Function function)
        {
            Invalidate(function.Address);
        }

        public Expression GetDecompiledCode(Function function)
        {
            return GetDecompiledCode(function.Address);
        }

        public Expression GetDecompiledCode(int functionAddress)
        {
            if (expressionCache.ContainsKey(functionAddress))
            {
                return expressionCache[functionAddress];
            }
            else
            {
                Expression expression = GetNewExpression(functionAddress);
                return expression;
            }
        }

        private Expression GetNewExpression(int functionAddress)
        {
            Expression expression = Expression.Empty;
            try
            {
                expression = decompiler.DecompileFunction(functionAddress);
            }
            catch
            {

            }
            expressionCache.Add(functionAddress, expression);
            return expression;
        }

        public string GetDecompiledCodeText(Function function)
        {
            return GetDecompiledCodeText(function.Address);
        }

        public string GetDecompiledCodeText(int functionAddress)
        {
            if (this.codeTextCache.ContainsKey(functionAddress))
            {
                return this.codeTextCache[functionAddress];
            }
            else
            {
                AddCodeTextAndExpressionMap(functionAddress);
                return this.codeTextCache[functionAddress];
            }
        }

        public ExpressionMap GetExpressionMap(Function function)
        {
            return GetExpressionMap(function.Address);
        }

        public ExpressionMap GetExpressionMap(int functionAddress)
        {
            if (this.expressionMapCache.ContainsKey(functionAddress))
            {
                return this.expressionMapCache[functionAddress];
            }
            else
            {
                AddCodeTextAndExpressionMap(functionAddress);
                return this.expressionMapCache[functionAddress];
            }
        }

        private void AddCodeTextAndExpressionMap(int functionAddress)
        {
            string codeText = "";
            ExpressionMap expressionMap = new ExpressionMap();

            var expression = GetDecompiledCode(functionAddress);
            if (expression == null || expression == Expression.Empty)
            {

            }
            else
            {
                var displayer = new ExpressionDisplayer(ainFile);
                try
                {
                    codeText = displayer.PrintExpression2(expression);
                    expressionMap = displayer.expressionMap;
                }
                catch (Exception)
                {

                }
            }
            this.codeTextCache[functionAddress] = codeText;
            this.expressionMapCache[functionAddress] = expressionMap;
        }
    }
}
