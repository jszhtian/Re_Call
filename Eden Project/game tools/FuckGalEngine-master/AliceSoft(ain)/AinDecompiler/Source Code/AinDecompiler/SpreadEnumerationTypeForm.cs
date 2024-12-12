using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace AinDecompiler
{
    public partial class SpreadEnumerationTypeForm : Form
    {
        AinFile ainFile;
        IVariable variable;
        EnumerationType enumerationType;

        public SpreadEnumerationTypeForm()
        {
            InitializeComponent();
        }

        public SpreadEnumerationTypeForm(IVariable variable, EnumerationType enumerationType) : this()
        {
            this.ainFile = variable.Root;
            this.variable = variable;
            this.enumerationType = enumerationType;
        }

        enum SpecialNode
        {
            DummyNode = -1,
            DirectCopiesRecursive = 0,
            DirectCopies,
            Writes,
            Reads,
            Invalid,
        }

        private void SpreadEnumerationTypeForm_Load(object sender, EventArgs e)
        {
            if (variable != null)
            {
                var rootNode = CreateRootNode(variable);
                this.resultsTreeView.Nodes.Add(rootNode);
            }
        }

        private TreeNode CreateRootNode(IVariable variable)
        {
            var treeNode = new TreeNode();
            treeNode.Tag = variable;
            string nodeText = GetVariableDescription(variable);
            treeNode.Text = nodeText;
            treeNode.Nodes.Add(CreateNode(SpecialNode.DirectCopiesRecursive));
            treeNode.Nodes.Add(CreateNode(SpecialNode.DirectCopies));
            treeNode.Nodes.Add(CreateNode(SpecialNode.Reads));
            treeNode.Nodes.Add(CreateNode(SpecialNode.Writes));
            return treeNode;
        }

        private TreeNode CreateNode(SpecialNode specialNode)
        {
            var treeNode = new TreeNode();
            treeNode.Tag = specialNode;
            switch (specialNode)
            {
                case SpecialNode.DummyNode:
                    treeNode.Text = "Dummy";
                    break;
                case SpecialNode.DirectCopiesRecursive:
                    treeNode.Text = "Direct copies (recursive)";
                    break;
                case SpecialNode.DirectCopies:
                    treeNode.Text = "Direct copies";
                    break;
                case SpecialNode.Reads:
                    treeNode.Text = "Other variables in operations reading this variable";
                    break;
                case SpecialNode.Writes:
                    treeNode.Text = "Other variables in operations writing to this variable";
                    break;
                default:
                    treeNode.Text = "Invalid Node";
                    specialNode = SpecialNode.Invalid;
                    treeNode.Tag = specialNode;
                    break;
            }
            if (specialNode != SpecialNode.DummyNode && specialNode != SpecialNode.Invalid)
            {
                treeNode.Nodes.Add(CreateNode(SpecialNode.DummyNode));
            }
            return treeNode;
        }

        private TreeNode CreateNode(IVariable variable)
        {
            var treeNode = new TreeNode();
            treeNode.Tag = variable;
            string nodeText = GetVariableDescription(variable);
            treeNode.Text = nodeText;
            treeNode.Nodes.Add(CreateNode(SpecialNode.DirectCopies));
            treeNode.Nodes.Add(CreateNode(SpecialNode.Reads));
            treeNode.Nodes.Add(CreateNode(SpecialNode.Writes));
            return treeNode;
        }

        private string GetVariableDescription(IVariable variable)
        {
            StringBuilder sb = new StringBuilder();
            var parentFunction = variable.Parent as Function;
            var parentStruct = variable.Parent as Struct;
            var parentLibraryFunction = variable.Parent as HllFunction;
            var parentFunctype = variable.Parent as FunctionType;
            if (parentFunction != null)
            {
                var variable_ = variable as Variable;
                if (variable.Index >= parentFunction.ParameterCount)
                {
                    sb.Append("Local Variable ");
                    sb.Append(variable.ToString());
                    sb.Append(" for function ");
                    sb.Append(parentFunction.GetActiveName());
                }
                else
                {
                    sb.Append("Function Parameter ");
                    sb.Append(variable.ToString());
                    sb.Append(" for function ");
                    sb.Append(parentFunction.GetActiveName());
                }
            }
            else if (parentStruct != null)
            {
                sb.Append("Struct Member ");
                sb.Append(variable.ToString());
                sb.Append(" for struct ");
                sb.Append(parentStruct.GetActiveName());
            }
            else if (parentLibraryFunction != null)
            {
                sb.Append("Function Parameter ");
                sb.Append(variable.ToString());
                sb.Append(" for library function ");
                sb.Append(parentLibraryFunction.GetActiveName());
            }
            else if (parentFunctype != null)
            {
                sb.Append("Function Parameter ");
                sb.Append(variable.ToString());
                sb.Append(" for Functype ");
                sb.Append(parentFunctype.GetActiveName());
            }
            else
            {
                if (variable is Function)
                {
                    sb.Append("Function ");
                    sb.Append(variable.ToString());
                }
                else if (variable is Struct)
                {
                    sb.Append("Struct ");
                    sb.Append(variable.GetActiveName());
                }
                else if (variable is HllFunction)
                {
                    sb.Append("Library Function ");
                    sb.Append(variable.ToString());
                }
                else if (variable is FunctionType)
                {
                    sb.Append("Functype ");
                    sb.Append(variable.ToString());
                }
                else if (variable is Global)
                {
                    sb.Append("Global Variable ");
                    sb.Append(variable.ToString());
                }
                else
                {
                    sb.Append("Variable ");
                    sb.Append(variable.ToString());
                }
            }
            return sb.ToString();
        }

        private void resultsTreeView_BeforeExpand(object sender, TreeViewCancelEventArgs e)
        {
            var treeView = sender as TreeView;
            var parentNode = e.Node;

            IVariable lookFor = null;
            SpecialNode parentNodeType = SpecialNode.Invalid;

            //does node contain a dummy node?
            var firstNode = parentNode.Nodes.OfType<TreeNode>().FirstOrDefault();
            if (firstNode != null && firstNode.Tag is SpecialNode && (SpecialNode)firstNode.Tag == SpecialNode.DummyNode)
            {
                //remove dummy node
                firstNode.Remove();

                if (parentNode.Tag is SpecialNode)
                {
                    parentNodeType = (SpecialNode)parentNode.Tag;
                    var parentNode2 = parentNode.Parent;
                    if (parentNode2 != null)
                    {
                        lookFor = parentNode2.Tag as IVariable;
                    }
                }
            }

            if (lookFor != null)
            {
                switch (parentNodeType)
                {
                    case SpecialNode.DirectCopiesRecursive:
                        {
                            var tracer = new VariableTracer(ainFile);
                            var results = tracer.TraceVariable(lookFor, VariableTraceMode.DirectCopiesRecursive);
                            var nodes = CreateNodes(results);
                            parentNode.Nodes.AddRange(nodes);
                        }
                        break;
                    case SpecialNode.DirectCopies:
                        {
                            var tracer = new VariableTracer(ainFile);
                            var results = tracer.TraceVariable(lookFor, VariableTraceMode.DirectCopies);
                            var nodes = CreateNodes(results);
                            parentNode.Nodes.AddRange(nodes);
                        }
                        break;
                    case SpecialNode.Reads:
                        {
                            var tracer = new VariableTracer(ainFile);
                            var results = tracer.TraceVariable(lookFor, VariableTraceMode.Reads);
                            var nodes = CreateNodes(results);
                            parentNode.Nodes.AddRange(nodes);
                        }
                        break;
                    case SpecialNode.Writes:
                        {
                            var tracer = new VariableTracer(ainFile);
                            var results = tracer.TraceVariable(lookFor, VariableTraceMode.Writes);
                            var nodes = CreateNodes(results);
                            parentNode.Nodes.AddRange(nodes);
                        }
                        break;
                }
            }
        }

        private TreeNode[] CreateNodes(IEnumerable<IVariable> results)
        {
            return results.Select(variable => CreateNode(variable)).ToArray();
        }

        private void spreadButton_Click(object sender, EventArgs e)
        {
            var nodes = GetNodes(resultsTreeView);
            bool didSomething = false;
            foreach (var node in nodes)
            {
                if (node.Checked)
                {
                    var variable = node.Tag as IVariable;
                    if (variable != null)
                    {
                        if (variable.DataType.IsInteger())
                        {
                            var metaData = ainFile.MetadataFile.Metadata.GetOrAddNew(variable);
                            metaData.EnumerationType = this.enumerationType;
                            didSomething = true;
                        }
                    }
                }
            }
            if (didSomething)
            {
                ainFile.SaveMetadata();
            }
        }

        private IEnumerable<TreeNode> GetNodes(TreeView treeView)
        {
            foreach (var node in treeView.Nodes.OfType<TreeNode>())
            {
                yield return node;
                foreach (var node2 in GetNodes(node))
                {
                    yield return node2;
                }
            }
        }

        private IEnumerable<TreeNode> GetNodes(TreeNode treeNode)
        {
            foreach (var node in treeNode.Nodes.OfType<TreeNode>())
            {
                yield return node;
                foreach (var node2 in GetNodes(node))
                {
                    yield return node2;
                }
            }
        }

    }
}
