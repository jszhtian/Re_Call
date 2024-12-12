#pragma		once

#if 0

#include	<tree>

template <typename TYPE>
class Ctree : public tree<TYPE>
{
};

#else

// ****************************************************************
// ツリー
// ================================================================
template <typename T> class C_tree;

// ****************************************************************
// ツリーのノード
// ================================================================
template <typename T> struct C_tree_node
{
	BWP<C_tree_node>	parent;			// 親
	BSP<C_tree_node>	first_child;	// 最初の子供
	BWP<C_tree_node>	last_child;		// 最後の子供
	BWP<C_tree_node>	left;			// 左隣
	BSP<C_tree_node>	right;			// 右隣
	T					data;			// データ
};

// ****************************************************************
// ツリー
// ================================================================
template <typename T> class C_tree
{
public:
	C_tree();

	class iterator_base;
	class left_iterator;
	class right_iterator;
	class child_iterator;
	class child_reverse_iterator;

	// ================================================================
	// デフォルトイテレータを設定
	// ----------------------------------------------------------------
	typedef	left_iterator	iterator;

	// ================================================================
	// 各種関数
	// ----------------------------------------------------------------
	bool	empty();
	void	clear();
	int		get_child_cnt(iterator_base itr) const;		// 子供の数を取得

	// ノードを削除する
	template <typename T_itr>
	T_itr	erase(T_itr itr);

	// ソート
	void	sort(child_iterator first, child_iterator last, bool deep = false);
	template <typename Tsort_func>
	void	sort(child_iterator first, child_iterator last, Tsort_func sort_func, bool deep = false);

	// ================================================================
	// イテレータを取得
	// ----------------------------------------------------------------
	iterator				begin() const;
	iterator				end() const;
	left_iterator			left_begin() const;
	left_iterator			left_end() const;
	right_iterator			right_begin() const;
	right_iterator			right_end() const;
	child_iterator			child_begin(iterator_base itr) const;
	child_iterator			child_end(iterator_base itr) const;
	child_reverse_iterator	child_reverse_begin(iterator_base itr) const;
	child_reverse_iterator	child_reverse_end(iterator_base itr) const;

	// ルートを作成
	iterator_base	create_root(T data);
	// 右隣のノードを追加
	iterator_base	insert(iterator_base itr, T data);		
	// 子供ノードを作成
	iterator_base	append_child(iterator_base itr, T data);

	// ================================================================
	// iterator base
	// ----------------------------------------------------------------
	class iterator_base
	{
		friend	C_tree<T>;	// ツリークラスは自由にアクセスできる

	public:
		iterator_base()	{}
		// ノードからイテレータの作成
		iterator_base(BSP< C_tree_node<T> > node);
		// 各種演算子
		T&		operator *() const;
		T*		operator ->() const;
		bool	operator ==(iterator_base& rhs) const;
		bool	operator !=(iterator_base& rhs) const;

		// イテレータを取得
		child_iterator	begin()	const	{	return m_node->first_child->right;	}
		child_iterator	end()	const	{	return m_node->last_child.lock();	}

	protected:
		BSP< C_tree_node<T> >	m_node;

		// 前後のイテレータを探す
		iterator_base	get_prev_iterator() const;
		iterator_base	get_next_iterator() const;
		iterator_base	get_left_iterator() const;
		iterator_base	get_right_iterator() const;
	};

	// ================================================================
	// iterator
	// ----------------------------------------------------------------
	typedef left_iterator	iterator;

	// ================================================================
	// left iterator
	// ----------------------------------------------------------------
	class left_iterator : public iterator_base
	{
		friend	C_tree<T>;	// ツリークラスは自由にアクセスできる

	public:
		left_iterator()	{}
		// ノードからイテレータの作成
		left_iterator(BSP< C_tree_node<T> > node) : iterator_base(node)	{}
		// 基底クラスからイテレータ作成
		left_iterator(iterator_base base) : iterator_base(base)	{}

		// 各種演算子
		left_iterator	operator ++();
		left_iterator	operator --();
		left_iterator	operator ++(int n);
		left_iterator	operator --(int n);
		left_iterator	operator +(int n);
		left_iterator	operator -(int n);
	};

	// ================================================================
	// right iterator
	// ----------------------------------------------------------------
	class right_iterator : public iterator_base
	{
		friend	C_tree<T>;	// ツリークラスは自由にアクセスできる

	public:
		right_iterator()	{}
		// ノードからイテレータの作成
		right_iterator(BSP< C_tree_node<T> > node) : iterator_base(node)	{}
		// 基底クラスからイテレータ作成
		right_iterator(iterator_base base) : iterator_base(base)	{}

		// 各種演算子
		right_iterator	operator ++();
		right_iterator	operator --();
		right_iterator	operator ++(int n);
		right_iterator	operator --(int n);
		right_iterator	operator +(int n);
		right_iterator	operator -(int n);
	};

	// ================================================================
	// child iterator
	// ----------------------------------------------------------------
	class child_iterator : public iterator_base
	{
		friend	C_tree<T>;	// ツリークラスは自由にアクセスできる

	public:
		child_iterator()	{}
		// ノードからイテレータの作成
		child_iterator(BSP< C_tree_node<T> > node) : iterator_base(node)	{}
		// 基底クラスからイテレータ作成
		child_iterator(iterator_base base) : iterator_base(base)	{}

		// 各種演算子
		child_iterator	operator ++();
		child_iterator	operator --();
		child_iterator	operator ++(int n);
		child_iterator	operator --(int n);
		child_iterator	operator +(int n);
		child_iterator	operator -(int n);
	};

	// ================================================================
	// child reverse iterator
	// ----------------------------------------------------------------
	class child_reverse_iterator : public iterator_base
	{
		friend	C_tree<T>;	// ツリークラスは自由にアクセスできる

	public:
		child_reverse_iterator()	{}
		// ノードからイテレータの作成
		child_reverse_iterator(BSP< C_tree_node<T> > node) : iterator_base(node)	{}
		// 基底クラスからイテレータ作成
		child_reverse_iterator(iterator_base base) : iterator_base(base)	{}

		// 各種演算子
		child_reverse_iterator	operator ++();
		child_reverse_iterator	operator --();
		child_reverse_iterator	operator ++(int n);
		child_reverse_iterator	operator --(int n);
		child_reverse_iterator	operator +(int n);
		child_reverse_iterator	operator -(int n);
	};


private:
	BSP< C_tree_node<T> >	head;	// 先頭のダミー
	BSP< C_tree_node<T> >	head2;	// 先頭のダミー
	BSP< C_tree_node<T> >	tail;	// 末尾のダミー
	BSP< C_tree_node<T> >	tail2;	// 末尾のダミー

	BSP< C_tree_node<T> >	create_node(T data);	// 有効なノードを作成する

	// 比較ファンクタ
	template <typename T_sort_func>
	class node_compare
	{
	public:
		node_compare(T_sort_func sort_func) : m_sort_func(sort_func)	{}

		bool operator ()(BSP< C_tree_node<T> > lhs, BSP< C_tree_node<T> > rhs) 
		{
			return m_sort_func(lhs->data, rhs->data);
		}
	private:
		T_sort_func	m_sort_func;
	};
};


// ****************************************************************
// ツリー：コンストラクタ
// ================================================================
template <typename T> C_tree<T>::C_tree()
{
	// 両端のダミーを作る
	head = BSP< C_tree_node<T> >(new C_tree_node<T>);
	head2 = BSP< C_tree_node<T> >(new C_tree_node<T>);
	tail = BSP< C_tree_node<T> >(new C_tree_node<T>);
	tail2 = BSP< C_tree_node<T> >(new C_tree_node<T>);

	// クリア
	clear();
}

// ****************************************************************
// ツリー：空判定
// ================================================================
template <typename T> bool C_tree<T>::empty()
{
	return head->right == tail;
}

// ****************************************************************
// ツリー：クリア
// ================================================================
template <typename T> void C_tree<T>::clear()
{
	// ダミーをお互いにつなぐ
	head->left = head2;
	head->right = tail;
	tail->left = head;
	tail->right = tail2;
	head2->right = head;
	tail2->left = tail;
}

// ****************************************************************
// ツリー：子供の数を取得
// ================================================================
template <typename T> int C_tree<T>::get_child_cnt(iterator_base itr) const
{
	int cnt = 0;
	for (child_iterator child_itr = child_begin(itr); child_itr != child_end(itr); ++child_itr)
		cnt ++;

	return cnt;
}

// ****************************************************************
// ツリー：ノードを削除する
// ================================================================
template <typename T> template <typename Titr> Titr C_tree<T>::erase(Titr itr)
{
	itr.m_node->right->left = itr.m_node->left;
	itr.m_node->left.lock()->right = itr.m_node->right;

	// 次のイテレータを返す
	return ++itr;
}

// ****************************************************************
// ツリー：ノードをソートする
// ================================================================
template <typename T> bool node_comp(BSP< C_tree_node<T> >& lhs, BSP< C_tree_node<T> >& rhs)
{
	return lhs->m_node.data < rhs->m_node.data;
}

template <typename T> void C_tree<T>::sort(child_iterator first, child_iterator last, bool deep)
{
	std::less<T> sort_func;
	sort(first, last, sort_func, deep);
}

template <typename T> template <typename Tsort_func> void C_tree<T>::sort(child_iterator first, child_iterator last, Tsort_func sort_func, bool deep)
{
	// 両端の先を記憶しておく
	child_iterator pre_first = first - 1;

	// 子供をソート
	if (deep)	{
		for (child_iterator itr = first; itr != last; ++itr)
			sort(itr.begin(), itr.end(), sort_func, deep);
	}

	// セットに全ノードを入れる
	std::multiset< BSP< Ctree_node<T> >, node_compare<Tsort_func> > node_set(sort_func);
	for (child_iterator itr = first; itr != last; ++itr)
		node_set.insert(itr.m_node);
	// ソートしたノードをつなげていく
	child_iterator link_itr = pre_first;
	for (std::multiset< BSP< Ctree_node<T> >, node_compare<Tsort_func> >::iterator n_itr = node_set.begin(); n_itr != node_set.end(); ++n_itr, ++link_itr)	{
		link_itr.m_node->right = *n_itr;
		link_itr.m_node->right->left = link_itr.m_node;
	}
	link_itr.m_node->right = last.m_node;
	last.m_node->left = link_itr.m_node;
}

// ****************************************************************
// ツリー：ノードを作成する
// ================================================================
template <typename T> typename BSP< C_tree_node<T> > C_tree<T>::create_node(T data)
{
	BSP< C_tree_node<T> > new_node(new C_tree_node<T>);
	BSP< C_tree_node<T> > head(new C_tree_node<T>);		// ダミー
	BSP< C_tree_node<T> > tail(new C_tree_node<T>);		// ダミー

	new_node->data = data;
	new_node->first_child = head;
	new_node->last_child = tail;
	head->parent = new_node;
	head->right = tail;
	tail->parent = new_node;
	tail->left = head;

	return new_node;
}

// ****************************************************************
// ツリー：指定のノードの位置にノードを挿入する
// ================================================================
template <typename T> typename C_tree<T>::iterator_base C_tree<T>::insert(iterator_base itr, T data)
{
	BSP< C_tree_node<T> > my_node = itr.m_node;
	BSP< C_tree_node<T> > new_node = create_node(data);

	// 新しいノードの設定
	new_node->left = my_node->left;
	new_node->right = my_node;
	new_node->parent = my_node->parent;
	// 左隣のノードの設定
	my_node->left.lock()->right = new_node;
	// 自身のノードの設定
	my_node->left = new_node;

	return iterator_base(new_node);
}

// ****************************************************************
// ツリー：ルートを作成する
// ================================================================
template <typename T> typename C_tree<T>::iterator_base C_tree<T>::create_root(T data)
{
	// ツリーの初期化
	clear();

	// 末尾の位置にルートを挿入する
	return insert(tail, data);
}

// ****************************************************************
// ツリー：あるノードの子供を作成する
// ================================================================
template <typename T> typename C_tree<T>::iterator_base C_tree<T>::append_child(iterator_base itr, T data)
{
	return insert(itr.m_node->last_child.lock(), data);
}

// ****************************************************************
// ツリー：イテレータの取得
// ================================================================
template <typename T> typename C_tree<T>::iterator C_tree<T>::begin() const
{
	return left_begin();
}
template <typename T> typename C_tree<T>::iterator C_tree<T>::end() const
{
	return left_end();
}
template <typename T> typename C_tree<T>::left_iterator C_tree<T>::left_begin() const
{
	return head->right;
}
template <typename T> typename C_tree<T>::left_iterator C_tree<T>::left_end() const
{
	return tail;
}
template <typename T> typename C_tree<T>::right_iterator C_tree<T>::right_begin() const
{
	return tail->left.lock();
}
template <typename T> typename C_tree<T>::right_iterator C_tree<T>::right_end() const
{
	return head;
}
template <typename T> typename C_tree<T>::child_iterator C_tree<T>::child_begin(iterator_base itr) const
{
	return itr.m_node->first_child->right;
}
template <typename T> typename C_tree<T>::child_iterator C_tree<T>::child_end(iterator_base itr) const
{
	return itr.m_node->last_child.lock();
}
template <typename T> typename C_tree<T>::child_reverse_iterator C_tree<T>::child_reverse_begin(iterator_base itr) const
{
	return itr.m_node->last_child.lock()->left.lock();
}
template <typename T> typename C_tree<T>::child_reverse_iterator C_tree<T>::child_reverse_end(iterator_base itr) const
{
	return itr.m_node->first_child;
}

// ****************************************************************
// イテレータベース：ノードからイテレータを作成
// ================================================================
template <typename T> C_tree<T>::iterator_base::iterator_base(BSP< C_tree_node<T> > node)
{
	m_node = node;
}

// ****************************************************************
// イテレータベース：前のイテレータを探す
// ================================================================
template <typename T> typename C_tree<T>::iterator_base C_tree<T>::iterator_base::get_prev_iterator() const
{
	// 有効な子供がいれば最後の子供を返す
	if (m_node->last_child.lock()->left.lock() != m_node->first_child)
		return iterator_base(m_node->last_child.lock()->left.lock());

	// 左隣を捜す（いなければ親に遡って行く）
	BSP< Ctree_node<T> > node = m_node;
	while (node)	{
		if (node->left.lock()->left.lock())
			return iterator_base(node->left.lock());
		node = node->parent.lock();
	}

	// ここは通らないはず
	return iterator_base();
}

// ****************************************************************
// イテレータベース：次のイテレータを探す
// ================================================================
template <typename T> typename C_tree<T>::iterator_base C_tree<T>::iterator_base::get_next_iterator() const
{
	// 有効な子供がいれば最初の子供を返す
	if (m_node->first_child->right != m_node->last_child.lock())
		return iterator_base(m_node->first_child->right);

	// 有効な右隣を捜す（いなければ親に遡って行く）
	BSP< Ctree_node<T> > node = m_node;
	while (node)	{
		if (node->right->right)
			return iterator_base(node->right);
		node = node->parent.lock();
	}

	// ここは通らないはず
	return iterator_base();
}

// ****************************************************************
// イテレータベース：左のイテレータを探す
// ================================================================
template <typename T> typename C_tree<T>::iterator_base C_tree<T>::iterator_base::get_left_iterator() const
{
	return iterator_base(m_node->left.lock());
}

// ****************************************************************
// イテレータベース：右のイテレータを探す
// ================================================================
template <typename T> typename C_tree<T>::iterator_base C_tree<T>::iterator_base::get_right_iterator() const
{
	return iterator_base(m_node->right);
}

// ****************************************************************
// イテレータベース：各種演算
// ================================================================
template <typename T> T* C_tree<T>::iterator_base::operator ->() const
{
	return &m_node->data;
}
template <typename T> T& C_tree<T>::iterator_base::operator *() const
{
	return m_node->data;
}
template <typename T> bool C_tree<T>::iterator_base::operator ==(iterator_base& rhs) const
{
	return m_node == rhs.m_node;
}
template <typename T> bool C_tree<T>::iterator_base::operator !=(iterator_base& rhs) const
{
	return m_node != rhs.m_node;
}

// ****************************************************************
// left iterator：各種演算
// ================================================================
template <typename T> typename C_tree<T>::left_iterator C_tree<T>::left_iterator::operator ++()
{
	*this = get_next_iterator();
	return *this;
}
template <typename T> typename C_tree<T>::left_iterator C_tree<T>::left_iterator::operator --()
{
	*this = get_prev_iterator();
	return *this;
}
template <typename T> typename C_tree<T>::left_iterator C_tree<T>::left_iterator::operator ++(int n)
{
	left_iterator itr = *this;
	*this = get_next_iterator();
	return itr;
}
template <typename T> typename C_tree<T>::left_iterator C_tree<T>::left_iterator::operator --(int n)
{
	left_iterator itr = *this;
	*this = get_prev_iterator();
	return itr;
}
template <typename T> typename C_tree<T>::left_iterator C_tree<T>::left_iterator::operator +(int n)
{
	left_iterator itr = *this;
	for (int i = 0; i < n; ++i, ++itr);
	return itr;
}
template <typename T> typename C_tree<T>::left_iterator C_tree<T>::left_iterator::operator -(int n)
{
	left_iterator itr = *this;
	for (int i = 0; i < n; ++i, --itr);
	return itr;
}

// ****************************************************************
// right iterator：各種演算
// ================================================================
template <typename T> typename C_tree<T>::right_iterator C_tree<T>::right_iterator::operator ++()
{
	*this = get_prev_iterator();
	return *this;
}
template <typename T> typename C_tree<T>::right_iterator C_tree<T>::right_iterator::operator --()
{
	*this = get_next_iterator();
	return *this;
}
template <typename T> typename C_tree<T>::right_iterator C_tree<T>::right_iterator::operator ++(int n)
{
	right_iterator itr = *this;
	*this = get_prev_iterator();
	return itr;
}
template <typename T> typename C_tree<T>::right_iterator C_tree<T>::right_iterator::operator --(int n)
{
	right_iterator itr = *this;
	*this = get_next_iterator();
	return itr;
}
template <typename T> typename C_tree<T>::right_iterator C_tree<T>::right_iterator::operator +(int n)
{
	right_iterator itr = *this;
	for (int i = 0; i < n; ++i, ++itr);
	return itr;
}
template <typename T> typename C_tree<T>::right_iterator C_tree<T>::right_iterator::operator -(int n)
{
	right_iterator itr = *this;
	for (int i = 0; i < n; ++i, --itr);
	return itr;
}

// ****************************************************************
// child iterator：各種演算
// ================================================================
template <typename T> typename C_tree<T>::child_iterator C_tree<T>::child_iterator::operator ++()
{
	*this = get_right_iterator();
	return *this;
}
template <typename T> typename C_tree<T>::child_iterator C_tree<T>::child_iterator::operator --()
{
	*this = get_left_iterator();
	return *this;
}
template <typename T> typename C_tree<T>::child_iterator C_tree<T>::child_iterator::operator ++(int n)
{
	left_iterator itr = *this;
	*this = get_right_iterator();
	return itr;
}
template <typename T> typename C_tree<T>::child_iterator C_tree<T>::child_iterator::operator --(int n)
{
	left_iterator itr = *this;
	*this = get_left_iterator();
	return itr;
}
template <typename T> typename C_tree<T>::child_iterator C_tree<T>::child_iterator::operator +(int n)
{
	child_iterator itr = *this;
	for (int i = 0; i < n; ++i, ++itr);
	return itr;
}
template <typename T> typename C_tree<T>::child_iterator C_tree<T>::child_iterator::operator -(int n)
{
	child_iterator itr = *this;
	for (int i = 0; i < n; ++i, --itr);
	return itr;
}

// ****************************************************************
// child reverse iterator：各種演算
// ================================================================
template <typename T> typename C_tree<T>::child_reverse_iterator C_tree<T>::child_reverse_iterator::operator ++()
{
	*this = get_left_iterator();
	return *this;
}
template <typename T> typename C_tree<T>::child_reverse_iterator C_tree<T>::child_reverse_iterator::operator --()
{
	*this = get_right_iterator();
	return *this;
}
template <typename T> typename C_tree<T>::child_reverse_iterator C_tree<T>::child_reverse_iterator::operator ++(int n)
{
	left_iterator itr = *this;
	*this = get_left_iterator();
	return itr;
}
template <typename T> typename C_tree<T>::child_reverse_iterator C_tree<T>::child_reverse_iterator::operator --(int n)
{
	left_iterator itr = *this;
	*this = get_right_iterator();
	return itr;
}
template <typename T> typename C_tree<T>::child_reverse_iterator C_tree<T>::child_reverse_iterator::operator +(int n)
{
	left_iterator itr = *this;
	for (int i = 0; i < n; ++i, ++itr);
	return itr;
}
template <typename T> typename C_tree<T>::child_reverse_iterator C_tree<T>::child_reverse_iterator::operator -(int n)
{
	child_reverse_iterator itr = *this;
	for (int i = 0; i < n; ++i, --itr);
	return itr;
}

#endif
