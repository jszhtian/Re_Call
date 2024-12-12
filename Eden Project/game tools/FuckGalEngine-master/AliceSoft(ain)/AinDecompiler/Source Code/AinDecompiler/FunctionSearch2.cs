using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.IO;
using System.Text;
using System.Windows.Forms;
using System.Globalization;

namespace AinDecompiler
{
    public enum SearchKind
    {
        Function,
        StructMember,
        String,
        Message,
        Global,
        Local,
        Int,
        LibraryCalls,
        SystemCalls,
    }

    public partial class FunctionSearchForm2 : Form
    {
        enum SpecialNode
        {
            Dummy,
            CalledBy,
            Calls,
        }

        int functionNumber = -1;
        SearchKind searchKind;
        AinFile ainFile;
        //AlternativeNames AlternativeNames = new AlternativeNames();
        //Decompiler decompiler;
        //ExpressionDisplayer displayer;

        public ExplorerForm _parent;
        public new ExplorerForm Parent
        {
            get
            {
                return _parent;
            }
            set
            {
                _parent = value;
                ainFile = _parent.ainFile;
                //decompiler = _parent.decompiler;
                //displayer = _parent.displayer;
            }
        }

        void CreateAutoCompleteItems()
        {
            var creator = new AutoCompleteItemsCreator(ainFile, searchKind, SearchQueryTextBox, functionNumber);
            creator.CreateAutoCompleteItems();
        }

        public class AutoCompleteItemsCreator
        {
            AinFile ainFile;
            int functionNumber;
            TextBox SearchQueryTextBox;
            SearchKind searchKind;

            public AutoCompleteItemsCreator(AinFile ainFile, SearchKind searchKind, TextBox searchQueryTextBox, int functionNumber)
            {
                this.ainFile = ainFile;
                this.searchKind = searchKind;
                this.functionNumber = functionNumber;
                this.SearchQueryTextBox = searchQueryTextBox;
            }

            public void CreateAutoCompleteItems()
            {
                SearchQueryTextBox.AutoCompleteMode = AutoCompleteMode.Suggest;
                SearchQueryTextBox.AutoCompleteSource = AutoCompleteSource.CustomSource;

                HashSet<string> names = new HashSet<string>();
                if (searchKind == SearchKind.Function)
                {
                    AddFunctionsToSet(names);
                }
                else if (searchKind == SearchKind.Global)
                {
                    AddGlobalsToSet(names);
                }
                else if (searchKind == SearchKind.String)
                {
                    AddStringsToSet(names);
                }
                else if (searchKind == SearchKind.StructMember)
                {
                    AddStructsToSet(names);
                }
                else if (searchKind == SearchKind.Local)
                {
                    AddLocalsToSet(names);
                }
                else if (searchKind == SearchKind.LibraryCalls)
                {
                    AddLibraryCallsToSet(names);
                }
                else if (searchKind == SearchKind.SystemCalls)
                {
                    AddSystemCallsToSet(names);
                }
                var stringCollection = new AutoCompleteStringCollection();
                stringCollection.AddRange(names.ToArray());
                SearchQueryTextBox.AutoCompleteCustomSource = stringCollection;
            }

            private void AddLibraryCallsToSet(HashSet<string> names)
            {
                foreach (var libary in ainFile.Libraries)
                {
                    string libraryName = libary.LibraryName;
                    foreach (var function in libary.Functions)
                    {
                        string functionName = function.Name;
                        names.Set(libraryName + "." + functionName);
                    }
                }
            }

            private void AddSystemCallsToSet(HashSet<string> names)
            {
                foreach (var function in AinFile.SystemCalls)
                {
                    string functionName = function.Name;
                    names.Set(functionName);
                }
            }

            private void AddLocalsToSet(HashSet<string> names)
            {
                if (functionNumber >= 0 && functionNumber < ainFile.Functions.Count)
                {
                    var function = ainFile.Functions[functionNumber];
                    foreach (var variable in function.Parameters)
                    {
                        string variableName = variable.Name;
                        string alternativeName = variable.GetAlternativeName();
                        names.Set(variableName);
                        names.Set(alternativeName);
                    }
                }
            }

            private void AddStructsToSet(HashSet<string> names)
            {
                foreach (var structInfo in ainFile.Structs)
                {
                    string structName = structInfo.Name;
                    string alternativeName = structInfo.GetAlternativeName();
                    names.Set(structName);
                    names.Set(alternativeName);
                    foreach (var member in structInfo.Members)
                    {
                        string memberName = member.Name;
                        string alternativeMemberName = member.GetAlternativeName();
                        memberName = structName + "." + memberName;
                        alternativeMemberName = alternativeName + "." + alternativeMemberName;
                        names.Set(memberName);
                        names.Set(alternativeMemberName);
                    }
                }
            }

            private void AddStringsToSet(HashSet<string> names)
            {
                foreach (var str in ainFile.Strings)
                {
                    string alternativeName = ainFile.AlternativeNames.GetOrDefault(str, str);
                    names.Set(str);
                    names.Set(alternativeName);
                }
            }

            private void AddGlobalsToSet(HashSet<string> names)
            {
                foreach (var global in ainFile.Globals)
                {
                    string prefix = global.GroupName ?? "";
                    string alternativePrefix = global.GetAlternativeGlobalGroupName();
                    if (prefix != "") { prefix += "."; }
                    if (alternativePrefix != "") { alternativePrefix += "."; }
                    string globalName = global.Name;
                    string globalNameWithPrefix = prefix + globalName;
                    names.Set(globalNameWithPrefix);
                    string alternaitveName = prefix + global.GetAlternativeName();
                    names.Set(alternaitveName);
                }
            }

            //private string GetGlobalPrefix(Global global)
            //{
            //    return ainFile.GlobalGroupNames.GetOrDefault(global.GroupIndex, "");
            //}

            private void AddFunctionsToSet(HashSet<string> names)
            {
                foreach (var function in ainFile.Functions)
                {
                    string functionName = function.Name.Replace("@", "::");
                    string alternativeName = function.GetAlternativeName().Replace("@", "::");
                    names.Set(functionName);
                    names.Set(alternativeName);
                }
            }
        }

        public FunctionSearchForm2(ExplorerForm parent, string searchQuery, SearchKind searchKind, int functionNumber)
            : this(parent, searchKind, functionNumber)
        {
            SearchQueryTextBox.Text = searchQuery;
            DoSearch();
        }

        private void DoSearch()
        {
            DoSearch(SearchQueryTextBox.Text);
            ResultsTreeView.Select();
        }

        private void DoSearch(string query)
        {
            ClearList();
            ResultsTreeView.BeginUpdate();
            try
            {
                if (searchKind == SearchKind.Function)
                {
                    var items = GetFunctionsMatchingQuery(query);
                    AddItems(items);
                }
                if (searchKind == SearchKind.Global)
                {
                    var items2 = GetGlobalsMatchingQuery(query);
                    AddItems(items2);
                }
                if (searchKind == SearchKind.Int)
                {
                    int item;
                    if (int.TryParse(query, NumberStyles.Integer, CultureInfo.InvariantCulture, out item))
                    {
                        AddItem(item);
                    }
                }
                if (searchKind == SearchKind.String)
                {
                    var items2 = GetStringsMatchingQuery(query);
                    AddItems(items2);
                }
                if (searchKind == SearchKind.Local)
                {
                    var items2 = GetLocalVariablesMatchingQuery(query);
                    AddItems(items2);

                }
                if (searchKind == SearchKind.StructMember)
                {
                    var items2 = GetStructMembersMatchingQuery(query);
                    AddItems(items2);
                }
                if (searchKind == SearchKind.Message)
                {
                    var items2 = GetMessagesMatchingQuery(query);
                    AddItems(items2);
                }
                if (searchKind == SearchKind.LibraryCalls)
                {
                    var items2 = GetLibraryCallsMatchingQuery(query);
                    AddItems(items2);
                }
                if (searchKind == SearchKind.SystemCalls)
                {
                    var items2 = GetSystemCallsMatchingQuery(query);
                    AddItems(items2);
                }
            }
            finally
            {
                ResultsTreeView.EndUpdate();
            }
        }

        private IEnumerable<Function> GetSystemCallsMatchingQuery(string query)
        {
            HashSet<Function> items = new HashSet<Function>();
            List<Function> exactMatches = new List<Function>();

            foreach (var function in AinFile.SystemCalls)
            {
                string functionName = function.Name;

                if (functionName.Contains(query, StringComparison.InvariantCultureIgnoreCase))
                {
                    if (functionName.Equals(query, StringComparison.InvariantCultureIgnoreCase))
                    {
                        exactMatches.Add(function);
                    }
                    else
                    {
                        items.Set(function);
                    }
                }
            }
            var items2 = exactMatches.Concat(items.OrderBy(i => i.Name));
            return items2;
        }

        private IEnumerable<HllFunction> GetLibraryCallsMatchingQuery(string query)
        {
            HashSet<HllFunction> items = new HashSet<HllFunction>();
            List<HllFunction> exactMatches = new List<HllFunction>();

            foreach (var library in ainFile.Libraries)
            {
                string libraryName = library.LibraryName;
                string alternativeLibraryName = library.GetAlternativeName();
                foreach (var function in library.Functions)
                {
                    string functionName = function.Name;
                    string alternativeFunctionName = function.GetAlternativeName();
                    string fullName = libraryName + "." + functionName;
                    string alternativeFullName = alternativeLibraryName + "." + alternativeFunctionName;

                    if (fullName.Contains(query, StringComparison.InvariantCultureIgnoreCase) ||
                        alternativeFullName.Contains(query, StringComparison.InvariantCultureIgnoreCase))
                    {
                        if (fullName.Equals(query, StringComparison.InvariantCultureIgnoreCase) ||
                            alternativeFullName.Equals(query, StringComparison.InvariantCultureIgnoreCase))
                        {
                            exactMatches.Add(function);
                        }
                        else
                        {
                            items.Set(function);
                        }
                    }
                }
            }
            var items2 = exactMatches.Concat(items.OrderBy(i => i.ParentLibrary.LibraryName + "." + i.Name));
            return items2;
        }

        private IEnumerable<IVariable> GetStructMembersMatchingQuery(string query)
        {
            HashSet<IVariable> items = new HashSet<IVariable>();
            List<IVariable> exactMatches = new List<IVariable>();

            foreach (var structInfo in ainFile.Structs)
            {
                string structName = structInfo.Name;
                string alternativeStructName = structInfo.GetAlternativeName();
                foreach (var member in structInfo.Members)
                {
                    string memberName = member.Name;
                    string alternativeMemberName = member.GetAlternativeName();
                    string fullName = structName + "." + memberName;
                    string alternativeFullName = alternativeStructName + "." + alternativeMemberName;

                    if (fullName.Contains(query, StringComparison.InvariantCultureIgnoreCase) ||
                        alternativeFullName.Contains(query, StringComparison.InvariantCultureIgnoreCase))
                    {
                        if (fullName.Equals(query, StringComparison.InvariantCultureIgnoreCase) ||
                            alternativeFullName.Equals(query, StringComparison.InvariantCultureIgnoreCase))
                        {
                            exactMatches.Add(member);
                        }
                        else
                        {
                            items.Set(member);
                        }
                    }
                }
            }
            var items2 = exactMatches.Concat(items.OrderBy(i => i.Name));
            return items2;
        }

        private IEnumerable<IVariable> GetLocalVariablesMatchingQuery(string query)
        {
            HashSet<IVariable> items = new HashSet<IVariable>();
            List<IVariable> exactMatches = new List<IVariable>();

            if (functionNumber == -1)
            {
                return items;
            }

            foreach (IVariable variable in ainFile.Functions[functionNumber].Parameters)
            {
                string name = variable.Name;
                //string fullName = name;
                string alternativeName = variable.GetAlternativeName();
                //if (name != alternativeName)
                //{
                //    fullName += " (" + alternativeName + ")";
                //}
                if (name.Contains(query, StringComparison.InvariantCultureIgnoreCase) ||
                    alternativeName.Contains(query, StringComparison.InvariantCultureIgnoreCase))
                {
                    if (name.Equals(query, StringComparison.InvariantCultureIgnoreCase) ||
                        alternativeName.Equals(query, StringComparison.InvariantCultureIgnoreCase))
                    {
                        exactMatches.Add(variable);
                    }
                    else
                    {
                        items.Set(variable);
                    }
                }

            }
            var items2 = exactMatches.Concat(items.OrderBy(i => i.Name));
            return items2;
        }

        private IEnumerable<string> GetStringsMatchingQuery(string query)
        {
            HashSet<string> items = new HashSet<string>();
            List<string> exactMatches = new List<string>();

            if (String.IsNullOrEmpty(query))
            {
                return items;
            }

            foreach (var str in ainFile.Strings)
            {
                string name = str;
                //string fullName = str;
                string alternativeName = ainFile.AlternativeNames.GetOrDefault(name, name);
                //if (alternativeName != name)
                //{
                //    fullName += " (" + alternativeName + ")";
                //}
                if (name.Contains(query, StringComparison.InvariantCultureIgnoreCase) ||
                    alternativeName.Contains(query, StringComparison.InvariantCultureIgnoreCase))
                {
                    if (name.Equals(query, StringComparison.InvariantCultureIgnoreCase) ||
                        alternativeName.Equals(query, StringComparison.InvariantCultureIgnoreCase))
                    {
                        exactMatches.Add(name);
                    }
                    else
                    {
                        items.Set(name);
                    }
                }
            }
            var items2 = exactMatches.Concat(items.OrderBy(s => s));
            return items2;
        }

        private IEnumerable<string> GetMessagesMatchingQuery(string query)
        {
            HashSet<string> items = new HashSet<string>();
            if (String.IsNullOrEmpty(query))
            {
                return items;
            }

            foreach (var str in ainFile.Messages)
            {
                string message = str;
                if (message.Contains(query, StringComparison.InvariantCultureIgnoreCase))
                {
                    items.Set(message);
                }
            }
            return items;
        }

        private IEnumerable<Global> GetGlobalsMatchingQuery(string query)
        {
            HashSet<Global> items = new HashSet<Global>();
            List<Global> exactMatches = new List<Global>();

            foreach (var global in ainFile.Globals)
            {
                string prefix = global.GroupName ?? "";
                string alternativePrefix = global.GetAlternativeGlobalGroupName();
                string globalName = global.Name;
                string alternativeName = global.GetAlternativeName();

                if (prefix != "") { prefix += "."; }
                if (alternativePrefix != "") { alternativePrefix += "."; }
                string globalFullName = prefix + globalName;
                string alternativeFullName = alternativePrefix + alternativeName;
                if (globalFullName.Contains(query, StringComparison.InvariantCultureIgnoreCase) || alternativeFullName.Contains(query, StringComparison.InvariantCultureIgnoreCase))
                {
                    if (globalName.Equals(query, StringComparison.InvariantCultureIgnoreCase) ||
                        alternativeName.Equals(query, StringComparison.InvariantCultureIgnoreCase) ||
                        globalFullName.Equals(query, StringComparison.InvariantCultureIgnoreCase) ||
                        alternativeFullName.Equals(query, StringComparison.InvariantCultureIgnoreCase))
                    {
                        exactMatches.Add(global);
                    }
                    else
                    {
                        items.Set(global);
                    }
                }
            }
            var items2 = exactMatches.Concat(items.OrderBy(g => g.Name));
            return items2;
        }

        private IEnumerable<Function> GetFunctionsMatchingQuery(string query)
        {
            //replace "::" with "@" in query
            query = query.Replace("::", "@");
            {
                var split = query.Split('@');
                if (split.Length > 1)
                {
                    if (split[0].Equals(split[1], StringComparison.OrdinalIgnoreCase))
                    {
                        query = split[0] + "@0";
                    }
                    else if (split[1].Equals("~" + split[0], StringComparison.OrdinalIgnoreCase))
                    {
                        query = split[0] + "@1";
                    }
                }
            }
            //try exact match first
            Function exactMatch = null;
            List<Function> firstMatches = new List<Function>();
            if (ainFile.FunctionNameToIndex.ContainsKey(query))
            {
                int index = ainFile.FunctionNameToIndex[query];
                exactMatch = ainFile.Functions[index];
                firstMatches.Add(ainFile.Functions[index]);
            }
            if (ainFile.AlternativeNames.ContainsKey(query))
            {
                string alternativeName = ainFile.AlternativeNames[query];
                if (ainFile.FunctionNameToIndex.ContainsKey(alternativeName))
                {
                    int index = ainFile.FunctionNameToIndex[alternativeName];
                    if (exactMatch != null)
                    {
                        exactMatch = ainFile.Functions[index];
                    }
                    firstMatches.Add(ainFile.Functions[index]);
                }
            }
            List<Function> list = new List<Function>();

            //check original functions list for partial matches
            for (int i = 0; i < ainFile.Functions.Count; i++)
            {
                var function = ainFile.Functions[i];
                if (function != exactMatch)
                {
                    string functionName = function.Name;
                    if (functionName.Contains(query, StringComparison.InvariantCultureIgnoreCase))
                    {
                        list.Add(function);
                    }
                    string alternativeName = function.GetAlternativeName();
                    if (alternativeName.Contains(query, StringComparison.InvariantCultureIgnoreCase))
                    {
                        list.Add(function);
                    }
                }
            }

            var items = SortItemsByName(list);
            return firstMatches.Concat(items);
        }

        private void AddItems(IEnumerable<Function> items)
        {
            foreach (var item in items)
            {
                AddItem(item);
            }
        }

        private void AddItems(IEnumerable<Global> items)
        {
            foreach (var item in items)
            {
                AddItem(item);
            }
        }

        private void AddItems(IEnumerable<IVariable> items)
        {
            foreach (var item in items)
            {
                AddItem(item);
            }
        }

        private void AddItems(IEnumerable<HllFunction> items)
        {
            foreach (var item in items)
            {
                AddItem(item);
            }
        }

        private void AddItems(IEnumerable<int> items)
        {
            foreach (var item in items)
            {
                AddItem(item);
            }
        }

        private void AddItems(IEnumerable<string> items)
        {
            foreach (var item in items)
            {
                AddItem(item);
            }
        }


        private static IEnumerable<Function> SortItemsByName(IEnumerable<Function> list)
        {
            return list.OrderBy(func => func.Name).Distinct();
        }

        private void ClearList()
        {
            ResultsTreeView.Nodes.Clear();
        }

        private void AddItem(Function function)
        {
            AddToItem(ResultsTreeView.Nodes, function);
        }

        private void AddItem(int intValue)
        {
            AddToItem(ResultsTreeView.Nodes, intValue);
        }

        private void AddItem(string stringValue)
        {
            AddToItem(ResultsTreeView.Nodes, stringValue);
        }

        private void AddItem(Global global)
        {
            AddToItem(ResultsTreeView.Nodes, global);
        }

        private void AddItem(IVariable variable)
        {
            AddToItem(ResultsTreeView.Nodes, variable);
        }

        private void AddItem(HllFunction libraryFunction)
        {
            AddToItem(ResultsTreeView.Nodes, libraryFunction);
        }

        private string GetNodeText(IFunction function)
        {
            string nodeText;
            string functionDeclaration = ExpressionDisplayer.GetFunctionDeclaration(function);
            string alternativeDeclaration = ainFile.AlternativeNames.GetAlternativeFunctionDeclaration(function);

            if (functionDeclaration != alternativeDeclaration)
            {
                nodeText = functionDeclaration + " (" + alternativeDeclaration + ")";
            }
            else
            {
                nodeText = functionDeclaration;
            }

            return nodeText;
        }

        private string GetNodeText(Global global)
        {
            string nodeText;
            string prefix = ainFile.GlobalGroupNames.GetOrDefault(global.GroupIndex, "");
            string alternativePrefix = global.GetAlternativeGlobalGroupName();
            if (prefix != "") { prefix += "."; }
            if (alternativePrefix != "") { alternativePrefix += "."; }

            string globalName = global.Name;
            string alternativeName = global.GetAlternativeName();

            nodeText = prefix + globalName;
            if (alternativeName != globalName)
            {
                nodeText += " (" + alternativePrefix + alternativeName + ")";
            }

            nodeText = global.GetDataTypeName() + " " + nodeText;
            return nodeText;
        }

        private string GetNodeText(IVariable variable)
        {
            var parent = variable.Parent;
            var parentFunction = parent as Function;
            var parentStruct = parent as Struct;

            //string variableTypeString = ExpressionDisplayer.GetDataTypeName(variable);

            if (parentFunction != null)
            {
                string variableName = variable.Name;
                string alternativeName = variable.GetAlternativeName();
                string nodeText = variableName;
                if (alternativeName != variableName)
                {
                    nodeText += " (" + alternativeName + ")";
                }
                return nodeText;
            }
            if (parentStruct != null)
            {
                string structName = parentStruct.Name;
                string alternativeStructName = parentStruct.GetAlternativeName();
                string variableName = variable.Name;
                string alternativeVariableName = variable.GetAlternativeName();

                string nodeText = structName + "." + variableName;
                string alternativeNodeText = alternativeStructName + "." + alternativeVariableName;
                if (alternativeNodeText != nodeText)
                {
                    nodeText += " (" + alternativeNodeText + ")";
                }
                return nodeText;
            }
            return variable.Name;
        }

        private string GetNodeText(string stringValue)
        {
            string alternativeName = ainFile.AlternativeNames.GetOrDefault(stringValue, stringValue);
            if (alternativeName != stringValue)
            {
                return stringValue + " ( " + alternativeName + ")";
            }
            else
            {
                return stringValue;
            }
        }

        private string GetNodeText(int intValue)
        {
            return intValue.ToString();
        }

        public FunctionSearchForm2(ExplorerForm parent, SearchKind searchKind, int functionNumber)
            : this()
        {
            this.functionNumber = functionNumber;
            this.Parent = parent;

            SetSearchKind(searchKind);
        }

        private void SetSearchKind(SearchKind searchKind)
        {
            this.searchKind = searchKind;
            ignoreComboBoxEvent = true;
            this.searchKindComboBox.SelectedItem = searchKind;
            ignoreComboBoxEvent = false;
            switch (searchKind)
            {
                case SearchKind.Function:
                    this.Text = "Search for function";
                    break;
                case SearchKind.Global:
                    this.Text = "Search for global variable";
                    break;
                case SearchKind.Int:
                    this.Text = "Search for int value";
                    break;
                case SearchKind.Local:
                    this.Text = "Search for local variable";
                    break;
                case SearchKind.String:
                    this.Text = "Search for string value";
                    break;
                case SearchKind.StructMember:
                    this.Text = "Search for struct member";
                    break;
                case SearchKind.Message:
                    this.Text = "Search for message";
                    break;
                case SearchKind.LibraryCalls:
                    this.Text = "Search for library calls";
                    break;
                case SearchKind.SystemCalls:
                    this.Text = "Search for system calls";
                    break;
            }

            CreateAutoCompleteItems();
        }

        public FunctionSearchForm2()
        {
            InitializeComponent();

            ignoreComboBoxEvent = true;
            SearchKind[] searchKinds = (SearchKind[])Enum.GetValues(typeof(SearchKind));
            searchKindComboBox.DataSource = searchKinds;
            searchKindComboBox.SelectedItem = searchKind;
            ignoreComboBoxEvent = false;
        }

        private void FunctionSearch_Load(object sender, EventArgs e)
        {

        }

        private void SearchButton_Click(object sender, EventArgs e)
        {
            DoSearch();
        }

        private void ResultsTreeView_BeforeExpand(object sender, TreeViewCancelEventArgs e)
        {
            ResultsTreeView.BeginUpdate();
            var nodeToExpand = e.Node;
            if (nodeToExpand != null)
            {
                ExpandNode(nodeToExpand);
            }
        }

        private void ExpandNode(TreeNode nodeToExpand)
        {
            try
            {
                if (nodeToExpand.Nodes.Count == 1)
                {
                    var childNode = nodeToExpand.Nodes[0];
                    if (childNode.Tag as SpecialNode? == SpecialNode.Dummy)
                    {
                        nodeToExpand.Nodes.RemoveAt(0);

                        if (nodeToExpand.Tag as SpecialNode? == SpecialNode.CalledBy)
                        {
                            var parentNode = nodeToExpand.Parent;
                            if (parentNode != null)
                            {
                                var function = parentNode.Tag as Function;
                                var libraryCall = parentNode.Tag as HllFunction;
                                var intValue = parentNode.Tag as int?;
                                var stringValue = parentNode.Tag as string;
                                var global = parentNode.Tag as Global;
                                var variable = parentNode.Tag as IVariable;

                                if (function != null)
                                {
                                    if (function.Name.StartsWith("system."))
                                    {
                                        var calledBy = GetCalledBySystemCall(function);
                                        AddToItem(nodeToExpand.Nodes, calledBy);
                                    }
                                    else
                                    {
                                        var calledBy = GetCalledBy(function);
                                        AddToItem(nodeToExpand.Nodes, calledBy);
                                    }
                                }
                                else if (libraryCall != null)
                                {
                                    var calledBy = GetCalledBy(libraryCall);
                                    AddToItem(nodeToExpand.Nodes, calledBy);
                                }
                                else if (intValue != null)
                                {
                                    IEnumerable<Function> usedBy = GetFunctionUsingInt(intValue.Value);
                                    AddToItem(nodeToExpand.Nodes, usedBy);
                                }
                                else if (stringValue != null)
                                {
                                    IEnumerable<Function> usedBy = null;
                                    if (searchKind == SearchKind.String)
                                    {
                                        usedBy = GetFunctionsUsingString(stringValue);
                                    }
                                    if (searchKind == SearchKind.Message)
                                    {
                                        usedBy = GetFunctionsUsingMessage(stringValue);
                                    }

                                    AddToItem(nodeToExpand.Nodes, usedBy);
                                }
                                else if (global != null)
                                {
                                    IEnumerable<Function> usedBy = GetFunctionsUsingGlobal(global);
                                    AddToItem(nodeToExpand.Nodes, usedBy);
                                }
                                else if (variable != null)
                                {
                                    var parent = variable.Parent;
                                    var functionParent = parent as Function;
                                    var structParent = parent as Struct;
                                    IEnumerable<Function> usedBy = null;
                                    if (functionParent != null)
                                    {
                                        var usedBy2 = GetUsesOfLocal(variable);
                                        AddToItem(nodeToExpand.Nodes, usedBy2);
                                    }
                                    if (structParent != null)
                                    {
                                        usedBy = GetFunctionsUsingStructMember(variable);
                                    }
                                    AddToItem(nodeToExpand.Nodes, usedBy);
                                }
                            }
                        }
                        else if (nodeToExpand.Tag as SpecialNode? == SpecialNode.Calls)
                        {
                            var parentNode = nodeToExpand.Parent;
                            if (parentNode != null)
                            {
                                Function function = null;
                                function = parentNode.Tag as Function;
                                if (function != null)
                                {
                                    var callsFunctions = GetCallsFunctions(function);
                                    AddToItem(nodeToExpand.Nodes, callsFunctions);
                                }
                            }
                        }
                        else
                        {
                            var function = nodeToExpand.Tag as Function;
                            var intValue = nodeToExpand.Tag as int?;
                            var stringValue = nodeToExpand.Tag as string;
                            var global = nodeToExpand.Tag as Global;
                            var variable = nodeToExpand.Tag as IVariable;

                            if (function != null)
                            {
                                var calledByNode = new TreeNode("Called By:");
                                calledByNode.Tag = SpecialNode.CalledBy;
                                nodeToExpand.Nodes.Add(calledByNode);
                                var dummyNode1 = new TreeNode();
                                dummyNode1.Tag = SpecialNode.Dummy;
                                calledByNode.Nodes.Add(dummyNode1);

                                if (!function.Name.StartsWith("system."))
                                {
                                    var callsNode = new TreeNode("Calls Functions");
                                    callsNode.Tag = SpecialNode.Calls;
                                    nodeToExpand.Nodes.Add(callsNode);
                                    var dummyNode2 = new TreeNode();
                                    dummyNode2.Tag = SpecialNode.Dummy;
                                    callsNode.Nodes.Add(dummyNode2);
                                }
                            }
                            else if (intValue != null || stringValue != null || global != null || variable != null)
                            {
                                var usedByNode = new TreeNode("Used By:");
                                usedByNode.Tag = SpecialNode.CalledBy;
                                nodeToExpand.Nodes.Add(usedByNode);
                                var dummyNode1 = new TreeNode();
                                dummyNode1.Tag = SpecialNode.Dummy;
                                usedByNode.Nodes.Add(dummyNode1);
                            }
                        }
                    }

                }
            }
            finally
            {
                ResultsTreeView.EndUpdate();
            }
        }

        private IEnumerable<ExpressionMap.ExpressionListNode> GetUsesOfLocal(IVariable variable)
        {
            ExpressionMap expressionMap = null;
            if (this.Parent != null)
            {
                expressionMap = this.Parent.expressionMap;
            }
            if (expressionMap != null)
            {
                return expressionMap.FindVariable(variable);
            }
            else
            {
                return new ExpressionMap().FindVariable(null);
            }
        }

        private IEnumerable<Function> GetFunctionsUsingGlobal(Global global)
        {
            return ainFile.CodeSearchCache.GlobalsCache.GetUsedBy(global);

            //var searcher = new CodeSearcher(ainFile);
            //return searcher.FindGlobal(global.Index);

            //int globalNumber = global.Index;
            //Func<InstructionInfo, InstructionInfo, bool> condition = (ins1, ins2) =>
            //{
            //    if ((ins1.instruction == Instruction.SH_GLOBALREF && ins1.word1 == globalNumber) ||
            //        (ins1.instruction == Instruction.PUSHGLOBALPAGE && ins2.instruction == Instruction.PUSH && ins2.word1 == globalNumber))
            //    {
            //        return true;
            //    }
            //    else
            //    {
            //        return false;
            //    }
            //};
            //return GetFunctionsUsingSomething(condition);
        }

        private IEnumerable<Function> GetFunctionsUsingStructMember(IVariable structMember)
        {
            return ainFile.CodeSearchCache.GetFunctionsUsingStructMember(structMember);

            //var searcher = new CodeSearcher(ainFile);
            //return searcher.FindStructMember(structMember);

            //var parent = structMember.Parent;
            //var structParent = parent as Struct;
            //if (structParent == null)
            //{
            //    return null;
            //}

            //int functionStructNumber = -1;
            //int structNumber = structParent.Index;
            //int structMemberNumber = structMember.Index;
            //int functionNumber = -1;

            //bool decompileMe = false;

            //Function function = null;

            //Func<InstructionInfo, InstructionInfo, bool> condition = (ins1, ins2) =>
            //{
            //    if (functionStructNumber == structNumber)
            //    {
            //        if ((ins1.instruction == Instruction.SH_STRUCTREF && ins1.word1 == structMemberNumber) ||
            //            (ins1.instruction == Instruction.PUSHSTRUCTPAGE && ins2.word1 == structMemberNumber))
            //        {
            //            return true;
            //        }
            //    }
            //    IVariable referencedVariable = null;
            //    if (ins1.instruction == Instruction.SH_LOCALREF || (ins1.instruction == Instruction.PUSHLOCALPAGE && ins2.instruction == Instruction.PUSH))
            //    {
            //        int value;
            //        if (ins1.instruction == Instruction.SH_LOCALREF)
            //        {
            //            value = ins1.word1;
            //        }
            //        else
            //        {
            //            value = ins2.word1;
            //        }
            //        if (value >= 0 && value < function.Arguments.Length)
            //        {
            //            referencedVariable = function.Arguments[value];
            //        }
            //    }
            //    else if (ins1.instruction == Instruction.SH_GLOBALREF || (ins1.instruction == Instruction.PUSHGLOBALPAGE && ins2.instruction == Instruction.PUSH))
            //    {
            //        int value;
            //        if (ins1.instruction == Instruction.SH_LOCALREF)
            //        {
            //            value = ins1.word1;
            //        }
            //        else
            //        {
            //            value = ins2.word1;
            //        }
            //        if (value >= 0 && value < ainFile.Globals.Count)
            //        {
            //            referencedVariable = ainFile.Globals[value];
            //        }
            //    }
            //    if (referencedVariable != null && referencedVariable.StructType == structNumber)
            //    {
            //        decompileMe = true;
            //    }
            //    //if (ins2.nextAddress < ainFile.Code.Length)
            //    //var ins3 = Decompiler.Peek(ainFile.Code, ins2.nextAddress);


            //    return false;
            //};

            //string structName = structParent.Name;

            //List<Function> results = new List<Function>();
            //foreach (var function2 in ainFile.Functions)
            //{
            //    function = function2;
            //    functionNumber = function.Index;
            //    var functionName = function.Name;
            //    var split = functionName.Split('@');
            //    if (split.Length >= 2 && split[0] == structName)
            //    {
            //        functionStructNumber = structNumber;
            //    }
            //    else
            //    {
            //        functionStructNumber = -1;
            //    }

            //    decompileMe = false;
            //    bool result = false;
            //    if (DoesFunctionUseSomething(function, condition))
            //    {
            //        result = true;
            //    }
            //    else
            //    {
            //        if (decompileMe)
            //        {
            //            try
            //            {
            //                var decompiler = new Decompiler(ainFile);
            //                var expression = decompiler.DecompileFunction(function);

            //                Expression e = expression;
            //                while (e != null)
            //                {
            //                    if (e.Variable == structMember)
            //                    {
            //                        result = true;
            //                        break;
            //                    }
            //                    e = e.GetNextExpression();
            //                }

            //            }
            //            catch (InvalidOperationException)
            //            {

            //            }
            //            catch (NullReferenceException)
            //            {

            //            }
            //        }
            //    }
            //    if (result == true)
            //    {
            //        results.Add(function);
            //    }
            //}
            //return results;
        }

        private IEnumerable<Function> GetFunctionsUsingMessage(string messageValue)
        {
            return ainFile.CodeSearchCache.MessagesCache.GetUsedBy(messageValue);

            //var searcher = new CodeSearcher(this.ainFile);
            //return searcher.FindMessage(messageValue);

            //int messageNumber = Array.IndexOf(ainFile.Messages, stringValue);
            //Func<InstructionInfo, InstructionInfo, bool> condition = (ins1, ins2) =>
            //{
            //    if (ins1.instruction == Instruction.MSG && ins1.word1 == messageNumber)
            //    {
            //        return true;
            //    }
            //    else
            //    {
            //        return false;
            //    }
            //};
            //return GetFunctionsUsingSomething(condition);
        }

        private IEnumerable<Function> GetFunctionsUsingString(string stringValue)
        {
            return ainFile.CodeSearchCache.StringsCache.GetUsedBy(stringValue);

            //var searcher = new CodeSearcher(this.ainFile);
            //return searcher.FindString(stringValue);

            //int stringNumber = Array.IndexOf(ainFile.Strings, stringValue);
            //Func<InstructionInfo, InstructionInfo, bool> condition = (ins1, ins2) =>
            //{
            //    if (ins1.instruction == Instruction.S_PUSH && ins1.word1 == stringNumber)
            //    {
            //        return true;
            //    }
            //    else
            //    {
            //        return false;
            //    }
            //};
            //return GetFunctionsUsingSomething(condition);
        }

        private IEnumerable<Function> GetFunctionUsingInt(int intValue)
        {
            return ainFile.CodeSearchCache.IntsCache.GetUsedBy(intValue);

            //var searcher = new CodeSearcher(this.ainFile);
            //return searcher.FindInt(intValue);

            //Func<InstructionInfo, InstructionInfo, bool> condition = (ins1, ins2) =>
            //{
            //    if (ins1.instruction == Instruction.S_PUSH && ins1.word1 == intValue)
            //    {
            //        return true;
            //    }
            //    else
            //    {
            //        return false;
            //    }
            //};
            //return GetFunctionsUsingSomething(condition);
        }

        class CodeSearcher
        {
            public CodeSearcher(AinFile ainFile)
            {
                this.ainFile = ainFile;
            }

            AinFile ainFile;
            int currentStructType = -1;
            int previousStructType = -1;
            int currentFunction = -1;
            int currentClass = -1;
            IList<IVariable> locals;
            //IList<IVariable> members;

            public List<Function> FindInt(int intValue)
            {
                return FindSomething(ArgumentKind.Int, i =>
                {
                    return i == intValue;
                });
            }

            public List<Function> FindFunction(int functionIndex)
            {
                List<Function> result = new List<Function>();
                foreach (var function in ainFile.Functions)
                {
                    if (FindFunction(function, functionIndex))
                    {
                        result.Add(function);
                    }
                }
                return result;
                //return FindSomething(ArgumentKind.Function, i =>
                //{
                //    return i == functionIndex;
                //});
            }

            public List<Function> FindGlobal(int globalIndex)
            {
                return FindSomething(ArgumentKind.Global, i =>
                {
                    return i == globalIndex;
                });
            }

            public List<Function> FindString(string stringValue)
            {
                return FindSomething(ArgumentKind.String, i =>
                {
                    return i >= 0 && i < ainFile.Strings.Count && ainFile.Strings[i] == stringValue;
                });
            }

            public List<Function> FindMessage(string messageValue)
            {
                return FindSomething(ArgumentKind.Message, i =>
                {
                    return i >= 0 && i < ainFile.Messages.Count && ainFile.Messages[i] == messageValue;
                });
            }

            public List<Function> FindStructMember(IVariable structMember)
            {
                var structInfo = structMember.Parent as Struct;
                if (structInfo == null)
                {
                    throw new ArgumentException("argument is not a struct member");
                }
                int structTypeIndex = structInfo.Index;
                int memberIndex = structMember.Index;

                return FindSomething2(structMember);
            }

            public List<Function> FindStructMemberOld(IVariable structMember)
            {
                var structInfo = structMember.Parent as Struct;
                if (structInfo == null)
                {
                    throw new ArgumentException("argument is not a struct member");
                }
                int structTypeIndex = structInfo.Index;
                int memberIndex = structMember.Index;

                return FindSomething(ArgumentKind.Member, i =>
                {
                    return i == memberIndex && this.previousStructType == structTypeIndex;
                });
            }

            private List<Function> FindSomething2(IVariable structMember)
            {
                List<Function> result = new List<Function>();
                foreach (var function in ainFile.Functions)
                {
                    var expressionMap = ainFile.DecompiledCodeCache.GetExpressionMap(function);
                    var findResult = expressionMap.FindVariable(structMember);
                    if (findResult.FirstOrDefault() != null)
                    {
                        result.Add(function);
                    }
                }
                return result;
            }

            List<Function> FindSomething(ArgumentKind kindToFind, Func<int, bool> condition)
            {
                List<Function> result = new List<Function>();
                foreach (var function in ainFile.Functions)
                {
                    if (EnumerateCode(function, kindToFind, condition))
                    {
                        result.Add(function);
                    }
                }
                return result;
            }

            bool EnumerateCode(Function function, ArgumentKind kindToFind, Func<int, bool> condition)
            {
                int startAddress = function.Address; //skipping the FUNC instruction
                var decompiler = new Decompiler(ainFile);
                int endAddress = decompiler.FindEndAddress(function.Address);
                currentFunction = function.Index;
                var currentClassObject = function.GetClass();
                currentClass = currentClassObject != null ? currentClassObject.Index : -1;
                //members = function.GetClass().Members;
                locals = ((IFunction)function).Parameters;

                int address = startAddress;

                while (address < endAddress)
                {
                    var instructionInfo = Decompiler.Peek(ainFile.Code, address);
                    int currentAddress = instructionInfo.CurrentAddress;
                    address = instructionInfo.nextAddress;
                    switch (instructionInfo.instruction)
                    {
                        case Instruction.PUSHGLOBALPAGE:
                        case Instruction.PUSHLOCALPAGE:
                        case Instruction.PUSHSTRUCTPAGE:
                        case Instruction.SR_REF:
                        case Instruction.SR_REF2:
                        case Instruction.SR_REFREF:
                        case Instruction.SH_STRUCT_SR_REF:
                            {
                                if (EnumerateGenericInstruction(instructionInfo, 0, kindToFind, condition)) return true;
                                var nextInstruction = Decompiler.Peek(ainFile.Code, address);
                                if (nextInstruction.instruction == Instruction.PUSH)
                                {
                                    var overrideKind = ArgumentKind.Member;
                                    if (instructionInfo.instruction == Instruction.PUSHGLOBALPAGE) overrideKind = ArgumentKind.Global;
                                    if (instructionInfo.instruction == Instruction.PUSHLOCALPAGE) overrideKind = ArgumentKind.Local;
                                    if (instructionInfo.instruction == Instruction.PUSHSTRUCTPAGE) overrideKind = ArgumentKind.LocalMember;
                                    if (EnumerateGenericInstruction(nextInstruction, overrideKind, kindToFind, condition)) return true;
                                    address = nextInstruction.nextAddress;
                                }
                            }
                            break;
                        //case Instruction.CALLONJUMP:
                        //    {
                        //        if (HandleCallOnJump(instructionInfo, kindToFind, condition)) return true;
                        //    }
                        //    break;
                        default:
                            {
                                if (instructionInfo.totalArguments > 0)
                                {
                                    if (EnumerateGenericInstruction(instructionInfo, 0, kindToFind, condition)) return true;
                                }
                            }
                            break;
                    }
                }
                return false;
            }

            int libraryNumber;

            bool EnumerateGenericInstruction(InstructionInfo instructionInfo, ArgumentKind overrideKind, ArgumentKind kindToFind, Func<int, bool> condition)
            {
                if (instructionInfo.totalArguments > 0)
                {
                    if (ArgumentKinds.InstructionArgumentKinds.ContainsKey((int)instructionInfo.instruction))
                    {
                        var argumentKinds = ArgumentKinds.InstructionArgumentKinds[(int)instructionInfo.instruction];
                        for (int i = 0; i < instructionInfo.totalArguments; i++)
                        {
                            var argumentKind = argumentKinds[i];
                            if (overrideKind != 0)
                            {
                                argumentKind = overrideKind;
                            }
                            int word = instructionInfo.words[i];
                            switch (argumentKind)
                            {
                                case ArgumentKind.AssignInt:
                                    argumentKind = ArgumentKind.Int;
                                    break;
                                case ArgumentKind.Function:
                                    if (word >= 0 && word < ainFile.Functions.Count)
                                    {
                                        var func = ainFile.Functions[word];
                                        previousStructType = currentStructType;
                                        currentStructType = func.StructType;
                                    }
                                    break;
                                case ArgumentKind.Global:
                                    if (word >= 0 && word < ainFile.Globals.Count)
                                    {
                                        var global = ainFile.Globals[word];
                                        previousStructType = currentStructType;
                                        currentStructType = global.StructType;
                                    }
                                    break;
                                case ArgumentKind.Library:
                                    libraryNumber = word;
                                    break;
                                case ArgumentKind.LibraryFunction:
                                    currentStructType = -1;
                                    break;
                                case ArgumentKind.Local:
                                    if (word >= 0 && word < locals.Count)
                                    {
                                        var local = locals[word];
                                        previousStructType = currentStructType;
                                        currentStructType = local.StructType;
                                    }
                                    break;
                                case ArgumentKind.LocalMember:
                                    argumentKind = ArgumentKind.Member;
                                    currentStructType = currentClass;
                                    if (currentStructType >= 0 && currentStructType < ainFile.Structs.Count)
                                    {
                                        var structInfo = ainFile.Structs[currentStructType];
                                        if (word >= 0 && word < structInfo.Members.Count)
                                        {
                                            var member = structInfo.Members[word];
                                            previousStructType = currentStructType;
                                            currentStructType = member.StructType;
                                        }
                                    }
                                    break;
                                case ArgumentKind.Member:
                                    if (currentStructType >= 0 && currentStructType < ainFile.Structs.Count)
                                    {
                                        var structInfo = ainFile.Structs[currentStructType];
                                        if (word >= 0 && word < structInfo.Members.Count)
                                        {
                                            var member = structInfo.Members[word];
                                            previousStructType = currentStructType;
                                            currentStructType = member.StructType;
                                        }
                                    }
                                    break;
                                case ArgumentKind.StructType:
                                    currentStructType = word;
                                    break;
                                case ArgumentKind.SystemCall:
                                    currentStructType = -1;
                                    break;
                            }
                            if (argumentKind == kindToFind)
                            {
                                if (condition(word))
                                {
                                    return true;
                                }
                            }
                        }
                    }
                }
                return false;
            }

            bool FindFunction(Function function, int functionToFind)
            {
                int startAddress = function.Address; //skipping the FUNC instruction
                var decompiler = new Decompiler(ainFile);
                int endAddress = decompiler.FindEndAddress(function.Address);

                int address = startAddress;

                while (address < endAddress)
                {
                    var instructionInfo = Decompiler.Peek(ainFile.Code, address);
                    int currentAddress = instructionInfo.CurrentAddress;
                    address = instructionInfo.nextAddress;
                    switch (instructionInfo.instruction)
                    {
                        case Instruction.CALLFUNC:
                        case Instruction.CALLMETHOD:
                            if (instructionInfo.word1 == functionToFind)
                            {
                                return true;
                            }
                            break;
                        case Instruction.THISCALLMETHOD_NOPARAM:
                        case Instruction.SH_STRUCTREF_CALLMETHOD_NO_PARAM:
                        case Instruction.SH_STRUCTREF2_CALLMETHOD_NO_PARAM:
                            {
                                int word = instructionInfo.words[instructionInfo.totalArguments - 1];
                                if (word == functionToFind)
                                {
                                    return true;
                                }
                            }
                            break;
                        case Instruction.CALLONJUMP:
                            {
                                var previousInstruction = Decompiler.Peek(ainFile.Code, instructionInfo.CurrentAddress - 6);
                                if (previousInstruction.instruction == Instruction.S_PUSH)
                                {
                                    int stringIndex = previousInstruction.word1;
                                    if (stringIndex >= 0 && stringIndex < ainFile.Strings.Count)
                                    {
                                        string stringValue = ainFile.Strings[stringIndex];
                                        if (ainFile.FunctionNameToIndex.ContainsKey(stringValue))
                                        {
                                            int functionIndex = ainFile.FunctionNameToIndex[stringValue];
                                            if (functionToFind == functionIndex)
                                            {
                                                return true;
                                            }
                                        }
                                    }
                                }

                            }
                            break;
                    }
                }
                return false;
            }
        }




        //private bool DoesFunctionUseSomething(Function function, Func<InstructionInfo, InstructionInfo, bool> condition)
        //{
        //    int address = function.Address;
        //    while (true)
        //    {
        //        var instructionInfo = Decompiler.Peek(ainFile.Code, address);
        //        address = instructionInfo.nextAddress;
        //        var instructionInfo2 = Decompiler.Peek(ainFile.Code, address);
        //        if (condition(instructionInfo, instructionInfo2))
        //        {
        //            return true;
        //        }
        //        if (instructionInfo.instruction == Instruction.ENDFUNC || instructionInfo2.instruction == Instruction.FUNC)
        //        {
        //            return false;
        //        }
        //    }
        //}

        //private List<Function> GetFunctionsUsingSomething(Func<InstructionInfo, InstructionInfo, bool> condition)
        //{
        //    List<Function> results = new List<Function>();
        //    foreach (var function in ainFile.Functions)
        //    {
        //        if (DoesFunctionUseSomething(function, condition))
        //        {
        //            results.Add(function);
        //        }
        //    }
        //    return results;

        //}

        private void AddToItem(TreeNodeCollection collection, IEnumerable<Function> items)
        {
            if (items == null) return;
            foreach (var item in items)
            {
                AddToItem(collection, item);
            }
        }

        private void AddToItem(TreeNodeCollection collection, IEnumerable<int> items)
        {
            foreach (var item in items)
            {
                AddToItem(collection, item);
            }
        }

        private void AddToItem(TreeNodeCollection collection, IEnumerable<IVariable> items)
        {
            foreach (var item in items)
            {
                AddToItem(collection, item);
            }
        }

        private void AddToItem(TreeNodeCollection collection, IEnumerable<string> items)
        {
            foreach (var item in items)
            {
                AddToItem(collection, item);
            }
        }

        private void AddToItem(TreeNodeCollection collection, IEnumerable<Global> items)
        {
            foreach (var item in items)
            {
                AddToItem(collection, item);
            }
        }

        private void AddToItem(TreeNodeCollection collection, IEnumerable<ExpressionMap.ExpressionListNode> items)
        {
            foreach (var item in items)
            {
                AddToItem(collection, item);
            }
        }

        private void AddToItem(TreeNodeCollection collection, string nodeText, object tag)
        {
            var treeNode = new TreeNode();
            treeNode.Text = nodeText;
            treeNode.Tag = tag;
            var dummyNode = new TreeNode();
            dummyNode.Text = "";
            dummyNode.Tag = SpecialNode.Dummy;
            treeNode.Nodes.Add(dummyNode);
            collection.Add(treeNode);
        }

        private void AddToItem(TreeNodeCollection collection, Function function)
        {
            string nodeText = GetNodeText(function);
            AddToItem(collection, nodeText, function);
        }

        private void AddToItem(TreeNodeCollection collection, int intValue)
        {
            string nodeText = GetNodeText(intValue);
            AddToItem(collection, nodeText, intValue);
        }

        private void AddToItem(TreeNodeCollection collection, Global global)
        {
            string nodeText = GetNodeText(global);
            AddToItem(collection, nodeText, global);
        }

        private void AddToItem(TreeNodeCollection collection, IVariable variable)
        {
            string nodeText = GetNodeText(variable);
            AddToItem(collection, nodeText, variable);
        }

        private void AddToItem(TreeNodeCollection collection, HllFunction libraryFunction)
        {
            string nodeText = GetNodeText(libraryFunction);
            AddToItem(collection, nodeText, libraryFunction);
        }

        private void AddToItem(TreeNodeCollection collection, string stringValue)
        {
            string nodeText = GetNodeText(stringValue);
            AddToItem(collection, nodeText, stringValue);
        }

        private void AddToItem(TreeNodeCollection collection, ExpressionMap.ExpressionListNode item)
        {
            string nodeText = GetNodeText(item);
            AddToItem(collection, nodeText, item);
        }

        private string GetNodeText(StartEndList<Expression>.ExpressionListNode item)
        {
            //todo: include line text
            return "line: " + item.lineNumber + " column: " + item.column + "\t";
        }

        HashSet<Function> GetAllFunctionsCalledBy(Function startFunction)
        {
            HashSet<Function> set = new HashSet<Function>();
            int address = startFunction.Address;
            int lastAddress = ainFile.Code.Length;
            InstructionInfo previousInstructionInfo = default(InstructionInfo);
            if (address < lastAddress)
            {
                previousInstructionInfo = Decompiler.Peek(ainFile.Code, address);
            }
            while (address < lastAddress)
            {
                int functionIndex = -1;
                var instructionInfo = Decompiler.Peek(ainFile.Code, address);

                if (instructionInfo.instruction == Instruction.FUNC ||
                    instructionInfo.instruction == Instruction.ENDFUNC)
                {
                    break;
                }

                if (instructionInfo.instruction == Instruction.CALLFUNC ||
                    instructionInfo.instruction == Instruction.CALLMETHOD)
                {
                    functionIndex = instructionInfo.word1;
                }

                if (instructionInfo.instruction == Instruction.CALLONJUMP)
                {
                    if (previousInstructionInfo.instruction == Instruction.S_PUSH)
                    {
                        int stringIndex = previousInstructionInfo.word1;
                        if (stringIndex >= 0 && stringIndex < ainFile.Strings.Count)
                        {
                            string str = ainFile.Strings[stringIndex];
                            if (ainFile.FunctionNameToIndex.ContainsKey(str))
                            {
                                functionIndex = ainFile.FunctionNameToIndex[str];
                            }
                        }
                    }
                }

                if (instructionInfo.instruction == Instruction.FT_ASSIGNS)
                {
                    if (previousInstructionInfo.instruction == Instruction.PUSH)
                    {
                        int functionTypeNumber = previousInstructionInfo.word1;
                        if (functionTypeNumber >= 0 && functionTypeNumber < ainFile.FunctionTypes.Count)
                        {
                            var stringInstructionInfo = Decompiler.Peek(ainFile.Code, previousInstructionInfo.CurrentAddress - 6);
                            if (stringInstructionInfo.instruction == Instruction.S_PUSH)
                            {
                                int stringNumber = stringInstructionInfo.word1;
                                if (stringNumber >= 0 && stringNumber < ainFile.Strings.Count)
                                {
                                    string functionName = ainFile.Strings[stringNumber];
                                    if (ainFile.FunctionNameToIndex.ContainsKey(functionName))
                                    {
                                        var function = ainFile.Functions[ainFile.FunctionNameToIndex[functionName]];
                                        functionIndex = function.Index;
                                    }
                                }
                            }
                        }
                    }
                }

                if (functionIndex >= 0 && functionIndex < ainFile.Functions.Count)
                {
                    var function = ainFile.Functions[functionIndex];
                    if (!set.Contains(function))
                    {
                        set.Add(function);
                    }
                    functionIndex = -1;
                }
                address = instructionInfo.nextAddress;
                previousInstructionInfo = instructionInfo;
            }
            return set;
        }

        private IEnumerable<Function> GetCallsFunctions(Function functionToFind)
        {
            return ainFile.CodeSearchCache.FunctionsCache.GetUses(functionToFind);

            //return GetAllFunctionsCalledBy(functionToFind);
        }

        private IEnumerable<Function> GetCalledBy(HllFunction libraryFunctionToFind)
        {
            return ainFile.CodeSearchCache.LibraryCallsCache.GetUsedBy(libraryFunctionToFind);
        }

        private IEnumerable<Function> GetCalledBy(Function functionToFind)
        {
            return ainFile.CodeSearchCache.FunctionsCache.GetUsedBy(functionToFind);

            //var searcher = new CodeSearcher(ainFile);
            //return searcher.FindFunction(functionToFind.Index);

            //List<Function> results = new List<Function>();
            //foreach (var function in ainFile.Functions)
            //{
            //    var set = GetAllFunctionsCalledBy(function);
            //    if (set.Contains(functionToFind))
            //    {
            //        results.Add(function);
            //    }
            //}
            //return results;
        }

        private IEnumerable<Function> GetCalledBySystemCall(Function functionToFind)
        {
            return ainFile.CodeSearchCache.SystemCallsCache.GetUsedBy(functionToFind);
        }

        object GetSelectedTag()
        {
            var selectedNode = ResultsTreeView.SelectedNode;
            if (selectedNode != null)
            {
                var tag = selectedNode.Tag;
                return tag;
            }
            return null;
        }

        private void contextMenuStrip1_Opening(object sender, CancelEventArgs e)
        {
            bool proceed = HandleContextMenu();
            e.Cancel = !proceed;
        }

        bool HandleContextMenu()
        {
            var selectedNode = ResultsTreeView.SelectedNode;
            if (selectedNode == null)
            {
                return false;
            }
            var function = selectedNode.Tag as Function;
            if (function != null)
            {
                return true;
            }
            return false;
        }

        private void copyNameToClipboardToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var selectedTag = GetSelectedTag();
            var function = selectedTag as Function;
            if (function != null)
            {
                Clipboard.SetText(function.Name);
            }
        }

        private void showInExplorerToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var selectedTag = GetSelectedTag();
            var function = selectedTag as Function;
            if (function != null && !function.Name.StartsWith("system."))
            {
                if (this.Parent != null)
                {
                    this.Parent.NavigateToFunction(function);
                    this.Parent.Activate();
                }
            }
            else
            {
                var node = selectedTag as ExpressionMap.ExpressionListNode;
                if (node != null)
                {
                    Parent.GoToCharacterPosition(node.start);
                }
                else
                {
                    var variable = selectedTag as IVariable;
                    if (variable != null && variable.StructType != -1)
                    {
                        var structInfo = variable.GetStructType();
                        if (structInfo != null)
                        {
                            Parent.VisitClass(structInfo.Name, false);
                            Parent.Activate();
                        }
                    }
                }
            }
        }

        private void showInExplorerInNewTabToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var selectedTag = GetSelectedTag();
            var variable = selectedTag as IVariable;
            if (selectedTag as Function != null ||
                selectedTag as ExpressionMap.ExpressionListNode != null ||
                (variable != null && variable.GetStructType() != null))
            {
                var function = selectedTag as Function;
                if (function != null && function.Name.StartsWith("system."))
                {
                    return;
                }
                this.Parent.NewTab();
                showInExplorerToolStripMenuItem_Click(sender, e);
            }
        }

        private void ResultsTreeView_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right || e.Button == MouseButtons.Middle)
            {
                var control = sender as Control;
                if (control != null)
                {
                    var node = ResultsTreeView.GetNodeAt(e.X, e.Y);
                    if (node != null)
                    {
                        ResultsTreeView.SelectedNode = node;
                    }
                }
            }

            if (e.Button == MouseButtons.Middle)
            {
                showInExplorerInNewTabToolStripMenuItem_Click(sender, e);
            }
        }

        private void ResultsTreeView_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyData == Keys.Enter)
            {
                showInExplorerToolStripMenuItem_Click(sender, e);
            }
        }

        bool ignoreComboBoxEvent = false;
        private void searchKindComboBox_SelectedValueChanged(object sender, EventArgs e)
        {
            if (!ignoreComboBoxEvent)
            {
                SearchKind? comboBoxValue = searchKindComboBox.SelectedItem as SearchKind?;
                if (comboBoxValue != null)
                {
                    if (comboBoxValue.Value != searchKind)
                    {
                        SetSearchKind(comboBoxValue.Value);
                    }
                }
            }
        }

        //private void ResultsTreeView_MouseDoubleClick(object sender, MouseEventArgs e)
        //{
        //    showInExplorerToolStripMenuItem_Click(sender, e);
        //}
    }
}
