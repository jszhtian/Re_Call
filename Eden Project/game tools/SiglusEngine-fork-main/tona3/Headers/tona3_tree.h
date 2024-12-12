#pragma		once

#if 0

#include	<tree>

template <typename TYPE>
class Ctree : public tree<TYPE>
{
};

#else

// ****************************************************************
// �c���[
// ================================================================
template <typename T> class C_tree;

// ****************************************************************
// �c���[�̃m�[�h
// ================================================================
template <typename T> struct C_tree_node
{
	BWP<C_tree_node>	parent;			// �e
	BSP<C_tree_node>	first_child;	// �ŏ��̎q��
	BWP<C_tree_node>	last_child;		// �Ō�̎q��
	BWP<C_tree_node>	left;			// ����
	BSP<C_tree_node>	right;			// �E��
	T					data;			// �f�[�^
};

// ****************************************************************
// �c���[
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
	// �f�t�H���g�C�e���[�^��ݒ�
	// ----------------------------------------------------------------
	typedef	left_iterator	iterator;

	// ================================================================
	// �e��֐�
	// ----------------------------------------------------------------
	bool	empty();
	void	clear();
	int		get_child_cnt(iterator_base itr) const;		// �q���̐����擾

	// �m�[�h���폜����
	template <typename T_itr>
	T_itr	erase(T_itr itr);

	// �\�[�g
	void	sort(child_iterator first, child_iterator last, bool deep = false);
	template <typename Tsort_func>
	void	sort(child_iterator first, child_iterator last, Tsort_func sort_func, bool deep = false);

	// ================================================================
	// �C�e���[�^���擾
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

	// ���[�g���쐬
	iterator_base	create_root(T data);
	// �E�ׂ̃m�[�h��ǉ�
	iterator_base	insert(iterator_base itr, T data);		
	// �q���m�[�h���쐬
	iterator_base	append_child(iterator_base itr, T data);

	// ================================================================
	// iterator base
	// ----------------------------------------------------------------
	class iterator_base
	{
		friend	C_tree<T>;	// �c���[�N���X�͎��R�ɃA�N�Z�X�ł���

	public:
		iterator_base()	{}
		// �m�[�h����C�e���[�^�̍쐬
		iterator_base(BSP< C_tree_node<T> > node);
		// �e�퉉�Z�q
		T&		operator *() const;
		T*		operator ->() const;
		bool	operator ==(iterator_base& rhs) const;
		bool	operator !=(iterator_base& rhs) const;

		// �C�e���[�^���擾
		child_iterator	begin()	const	{	return m_node->first_child->right;	}
		child_iterator	end()	const	{	return m_node->last_child.lock();	}

	protected:
		BSP< C_tree_node<T> >	m_node;

		// �O��̃C�e���[�^��T��
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
		friend	C_tree<T>;	// �c���[�N���X�͎��R�ɃA�N�Z�X�ł���

	public:
		left_iterator()	{}
		// �m�[�h����C�e���[�^�̍쐬
		left_iterator(BSP< C_tree_node<T> > node) : iterator_base(node)	{}
		// ���N���X����C�e���[�^�쐬
		left_iterator(iterator_base base) : iterator_base(base)	{}

		// �e�퉉�Z�q
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
		friend	C_tree<T>;	// �c���[�N���X�͎��R�ɃA�N�Z�X�ł���

	public:
		right_iterator()	{}
		// �m�[�h����C�e���[�^�̍쐬
		right_iterator(BSP< C_tree_node<T> > node) : iterator_base(node)	{}
		// ���N���X����C�e���[�^�쐬
		right_iterator(iterator_base base) : iterator_base(base)	{}

		// �e�퉉�Z�q
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
		friend	C_tree<T>;	// �c���[�N���X�͎��R�ɃA�N�Z�X�ł���

	public:
		child_iterator()	{}
		// �m�[�h����C�e���[�^�̍쐬
		child_iterator(BSP< C_tree_node<T> > node) : iterator_base(node)	{}
		// ���N���X����C�e���[�^�쐬
		child_iterator(iterator_base base) : iterator_base(base)	{}

		// �e�퉉�Z�q
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
		friend	C_tree<T>;	// �c���[�N���X�͎��R�ɃA�N�Z�X�ł���

	public:
		child_reverse_iterator()	{}
		// �m�[�h����C�e���[�^�̍쐬
		child_reverse_iterator(BSP< C_tree_node<T> > node) : iterator_base(node)	{}
		// ���N���X����C�e���[�^�쐬
		child_reverse_iterator(iterator_base base) : iterator_base(base)	{}

		// �e�퉉�Z�q
		child_reverse_iterator	operator ++();
		child_reverse_iterator	operator --();
		child_reverse_iterator	operator ++(int n);
		child_reverse_iterator	operator --(int n);
		child_reverse_iterator	operator +(int n);
		child_reverse_iterator	operator -(int n);
	};


private:
	BSP< C_tree_node<T> >	head;	// �擪�̃_�~�[
	BSP< C_tree_node<T> >	head2;	// �擪�̃_�~�[
	BSP< C_tree_node<T> >	tail;	// �����̃_�~�[
	BSP< C_tree_node<T> >	tail2;	// �����̃_�~�[

	BSP< C_tree_node<T> >	create_node(T data);	// �L���ȃm�[�h���쐬����

	// ��r�t�@���N�^
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
// �c���[�F�R���X�g���N�^
// ================================================================
template <typename T> C_tree<T>::C_tree()
{
	// ���[�̃_�~�[�����
	head = BSP< C_tree_node<T> >(new C_tree_node<T>);
	head2 = BSP< C_tree_node<T> >(new C_tree_node<T>);
	tail = BSP< C_tree_node<T> >(new C_tree_node<T>);
	tail2 = BSP< C_tree_node<T> >(new C_tree_node<T>);

	// �N���A
	clear();
}

// ****************************************************************
// �c���[�F�󔻒�
// ================================================================
template <typename T> bool C_tree<T>::empty()
{
	return head->right == tail;
}

// ****************************************************************
// �c���[�F�N���A
// ================================================================
template <typename T> void C_tree<T>::clear()
{
	// �_�~�[�����݂��ɂȂ�
	head->left = head2;
	head->right = tail;
	tail->left = head;
	tail->right = tail2;
	head2->right = head;
	tail2->left = tail;
}

// ****************************************************************
// �c���[�F�q���̐����擾
// ================================================================
template <typename T> int C_tree<T>::get_child_cnt(iterator_base itr) const
{
	int cnt = 0;
	for (child_iterator child_itr = child_begin(itr); child_itr != child_end(itr); ++child_itr)
		cnt ++;

	return cnt;
}

// ****************************************************************
// �c���[�F�m�[�h���폜����
// ================================================================
template <typename T> template <typename Titr> Titr C_tree<T>::erase(Titr itr)
{
	itr.m_node->right->left = itr.m_node->left;
	itr.m_node->left.lock()->right = itr.m_node->right;

	// ���̃C�e���[�^��Ԃ�
	return ++itr;
}

// ****************************************************************
// �c���[�F�m�[�h���\�[�g����
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
	// ���[�̐���L�����Ă���
	child_iterator pre_first = first - 1;

	// �q�����\�[�g
	if (deep)	{
		for (child_iterator itr = first; itr != last; ++itr)
			sort(itr.begin(), itr.end(), sort_func, deep);
	}

	// �Z�b�g�ɑS�m�[�h������
	std::multiset< BSP< Ctree_node<T> >, node_compare<Tsort_func> > node_set(sort_func);
	for (child_iterator itr = first; itr != last; ++itr)
		node_set.insert(itr.m_node);
	// �\�[�g�����m�[�h���Ȃ��Ă���
	child_iterator link_itr = pre_first;
	for (std::multiset< BSP< Ctree_node<T> >, node_compare<Tsort_func> >::iterator n_itr = node_set.begin(); n_itr != node_set.end(); ++n_itr, ++link_itr)	{
		link_itr.m_node->right = *n_itr;
		link_itr.m_node->right->left = link_itr.m_node;
	}
	link_itr.m_node->right = last.m_node;
	last.m_node->left = link_itr.m_node;
}

// ****************************************************************
// �c���[�F�m�[�h���쐬����
// ================================================================
template <typename T> typename BSP< C_tree_node<T> > C_tree<T>::create_node(T data)
{
	BSP< C_tree_node<T> > new_node(new C_tree_node<T>);
	BSP< C_tree_node<T> > head(new C_tree_node<T>);		// �_�~�[
	BSP< C_tree_node<T> > tail(new C_tree_node<T>);		// �_�~�[

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
// �c���[�F�w��̃m�[�h�̈ʒu�Ƀm�[�h��}������
// ================================================================
template <typename T> typename C_tree<T>::iterator_base C_tree<T>::insert(iterator_base itr, T data)
{
	BSP< C_tree_node<T> > my_node = itr.m_node;
	BSP< C_tree_node<T> > new_node = create_node(data);

	// �V�����m�[�h�̐ݒ�
	new_node->left = my_node->left;
	new_node->right = my_node;
	new_node->parent = my_node->parent;
	// ���ׂ̃m�[�h�̐ݒ�
	my_node->left.lock()->right = new_node;
	// ���g�̃m�[�h�̐ݒ�
	my_node->left = new_node;

	return iterator_base(new_node);
}

// ****************************************************************
// �c���[�F���[�g���쐬����
// ================================================================
template <typename T> typename C_tree<T>::iterator_base C_tree<T>::create_root(T data)
{
	// �c���[�̏�����
	clear();

	// �����̈ʒu�Ƀ��[�g��}������
	return insert(tail, data);
}

// ****************************************************************
// �c���[�F����m�[�h�̎q�����쐬����
// ================================================================
template <typename T> typename C_tree<T>::iterator_base C_tree<T>::append_child(iterator_base itr, T data)
{
	return insert(itr.m_node->last_child.lock(), data);
}

// ****************************************************************
// �c���[�F�C�e���[�^�̎擾
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
// �C�e���[�^�x�[�X�F�m�[�h����C�e���[�^���쐬
// ================================================================
template <typename T> C_tree<T>::iterator_base::iterator_base(BSP< C_tree_node<T> > node)
{
	m_node = node;
}

// ****************************************************************
// �C�e���[�^�x�[�X�F�O�̃C�e���[�^��T��
// ================================================================
template <typename T> typename C_tree<T>::iterator_base C_tree<T>::iterator_base::get_prev_iterator() const
{
	// �L���Ȏq��������΍Ō�̎q����Ԃ�
	if (m_node->last_child.lock()->left.lock() != m_node->first_child)
		return iterator_base(m_node->last_child.lock()->left.lock());

	// ���ׂ�{���i���Ȃ���ΐe�ɑk���čs���j
	BSP< Ctree_node<T> > node = m_node;
	while (node)	{
		if (node->left.lock()->left.lock())
			return iterator_base(node->left.lock());
		node = node->parent.lock();
	}

	// �����͒ʂ�Ȃ��͂�
	return iterator_base();
}

// ****************************************************************
// �C�e���[�^�x�[�X�F���̃C�e���[�^��T��
// ================================================================
template <typename T> typename C_tree<T>::iterator_base C_tree<T>::iterator_base::get_next_iterator() const
{
	// �L���Ȏq��������΍ŏ��̎q����Ԃ�
	if (m_node->first_child->right != m_node->last_child.lock())
		return iterator_base(m_node->first_child->right);

	// �L���ȉE�ׂ�{���i���Ȃ���ΐe�ɑk���čs���j
	BSP< Ctree_node<T> > node = m_node;
	while (node)	{
		if (node->right->right)
			return iterator_base(node->right);
		node = node->parent.lock();
	}

	// �����͒ʂ�Ȃ��͂�
	return iterator_base();
}

// ****************************************************************
// �C�e���[�^�x�[�X�F���̃C�e���[�^��T��
// ================================================================
template <typename T> typename C_tree<T>::iterator_base C_tree<T>::iterator_base::get_left_iterator() const
{
	return iterator_base(m_node->left.lock());
}

// ****************************************************************
// �C�e���[�^�x�[�X�F�E�̃C�e���[�^��T��
// ================================================================
template <typename T> typename C_tree<T>::iterator_base C_tree<T>::iterator_base::get_right_iterator() const
{
	return iterator_base(m_node->right);
}

// ****************************************************************
// �C�e���[�^�x�[�X�F�e�퉉�Z
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
// left iterator�F�e�퉉�Z
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
// right iterator�F�e�퉉�Z
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
// child iterator�F�e�퉉�Z
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
// child reverse iterator�F�e�퉉�Z
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
