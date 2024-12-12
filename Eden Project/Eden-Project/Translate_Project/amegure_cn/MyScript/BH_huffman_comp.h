//base on Giga_pac_Assage
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include <direct.h>
#include <windows.h>

typedef unsigned char  unit8;
typedef unsigned short unit16;
typedef unsigned int   unit32;

struct bits
{
	unsigned long curbits;
	unsigned long curbyte;
	unsigned char cache;
	unsigned char *stream;
	unsigned long stream_length;
};

void bits_init(struct bits *bits, unsigned char *stream, unsigned long stream_length)
{
	memset(bits, 0, sizeof(*bits));
	bits->stream = stream;
	bits->stream_length = stream_length;
}

/* ��setval�����λ���õ������λ����ĵط���ʼ */
int bit_put_high(struct bits *bits, unsigned char setval)
{
	bits->curbits++;
	bits->cache |= (setval & 1) << (8 - bits->curbits);
	if (bits->curbits == 8)
	{
		if (bits->curbyte >= bits->stream_length)
			return -1;
		bits->stream[bits->curbyte++] = bits->cache;
		bits->curbits = 0;
		bits->cache = 0;
	}
	return 0;
}

/* ���մӸ��ֽڵ����ֽڵ�˳���setval�е�ֵ���õ������λ����ĵط���ʼ */
int bits_put_high(struct bits *bits, unsigned int req_bits, void *setval)
{
	unsigned int this_bits;
	unsigned int this_byte;
	unsigned int i;

	this_byte = req_bits / 8;
	this_bits = req_bits & 7;
	for (int k = (int)this_bits - 1; k >= 0; k--)
	{
		unsigned char bitval;

		bitval = !!(((unsigned char *)setval)[this_byte] & (1 << k));
		if (bit_put_high(bits, bitval))
			return -1;
	}
	this_bits = req_bits & ~7;
	this_byte--;
	for (i = 0; i < this_bits; i++)
	{
		unsigned char bitval;

		bitval = !!(((unsigned char *)setval)[this_byte - i / 8] & (1 << (7 - (i & 7))));
		if (bit_put_high(bits, bitval))
			return -1;
	}

	return 0;
}

void bits_flush(struct bits *bits)
{
	bits->stream[bits->curbyte] = bits->cache;
}

typedef struct huffman_node 
{	
	unit32 weight;
	unit8 ascii;
	unit32 code;			/* ����������ֵ */
	unit32 code_lengths;	/* ����������ֵ��λ�� */
	struct huffman_node *parent;
	struct huffman_node *left_child;
	struct huffman_node *right_child;
}huffman_node_t;

static void huffman1_node_encode(huffman_node_t *node, unsigned int code, unsigned int code_lengths)
{
	/* ���ȱ������½ǵ�Ҷ�ڵ㿪ʼ */
	if (node->left_child)
	{
		code <<= 1;
		code_lengths++;
		huffman1_node_encode(node->left_child, code, code_lengths);
		code |= 1;
		huffman1_node_encode(node->right_child, code, code_lengths);
	}
	else
	{	/* ҳ��� */
		node->code = code;
		node->code_lengths = code_lengths;	
	}
}

static int huffman_code_tree_encode(struct bits *bits, huffman_node_t *parent)
{
	if (parent->left_child)
	{
		if (bit_put_high(bits, 1))	/* ��ʾ��ǰ���ڵ��к��� */
			return -1;
		
		if (huffman_code_tree_encode(bits, parent->left_child))
			return -1;
		
		if (huffman_code_tree_encode(bits, parent->right_child))
			return -1;
	}
	else
	{	/* ҳ�ڵ� */
		if (bit_put_high(bits, 0))	/* λ0��־��ʾ��ҳ�ڵ� */
			return -1;	
		
		/* д��ҳ�ڵ��asciiֵ */
		if (bits_put_high(bits, 8, (unsigned char *)&parent->ascii))
			return -1;		
	}
	return 0;
}

static huffman_node_t *huffman_child_init(huffman_node_t *child_node, unsigned int is_right_child)
{
#ifdef MY_CUSTOM_0
	child_node->code = is_right_child;	/* ˳����ϱ���ֵ��ֻ������(0)����(1)) */
	child_node->code_lengths = 1;
#endif
	
	return child_node;
}

static unsigned int huffman_tree_create(huffman_node_t *nodes)
{
	huffman_node_t *pnodes[256],*pnode = 0;	
	int leaves_node;				/* ��Ч��Ҷ������ */
	int parent_node;				/* �ϲ�ʱ�½���λ������ */
	int child_node;					/* Ҷ���λ�ü��� */
	int i;
	
	/* �����ֹ���(Ȩֵ��Ϊ0��)Ҷ�ڵ������� */
	for (i = 0; nodes[i].weight && i < 256; i++)
		pnodes[i] = &nodes[i];

	leaves_node = i;

	if (leaves_node < 2)
	{
		printf("��Ч��Ҷ�����Ŀ����\n");
		return -1;
	}

	parent_node = leaves_node;
	child_node = parent_node - 1;	
	while (child_node > 0)
	{
		pnode = &nodes[parent_node++];	/* �ϲ�����Ҷ����Ժ���½�� */
		/* CUSTOM!! */
		pnode->left_child = huffman_child_init(pnodes[child_node--], 0);	/* ��1��child�����Ϊ���� */
		pnode->right_child = huffman_child_init(pnodes[child_node--], 1);	/* ��2��child�����Ϊ�ҽ�� */
		pnode->left_child->parent = pnode->right_child->parent = pnode;		/* �½���Ϊ����� */
		pnode->weight = pnode->left_child->weight + pnode->right_child->weight;/* �����ȨֵΪ2�����ӵ�Ȩֵ֮�� */
		/* �ҵ�һ�����ʵĲ����, ����������ʣ������ɵ�ɭ���� */
		for (i = child_node; i >= 0; i--)
		{
			/* �ҵ�һ�����ʵĲ���� */
			/* custom!! */
			if (pnodes[i]->weight >= pnode->weight)
				break;
		}
		/* ���µĽڵ�������λ�� */		
		memmove(pnodes + i + 2, pnodes + i + 1, (child_node - i) * sizeof(huffman_node_t *));
		pnodes[i + 1] = pnode;
		child_node++;
	}
	/* pnode���Ǹ���� */
	/* �������������һ�����������е�2n - 1�����Ķ���pnodes */
	huffman1_node_encode(pnode, 0, 0);

	return leaves_node;	
}

/* ���������� */
static int huffman_weight_compare(const void *node1, const void *node2)
{
	huffman_node_t *nodes[2] = { (huffman_node_t *)node1, (huffman_node_t *)node2 };
	
	/* ����Ƚϵ�ǰ��2��˳�������������������� */
	return (int)nodes[1]->weight - (int)nodes[0]->weight;
}

static int huffman_ascii_compare(const void *node1, const void *node2)
{
	huffman_node_t *nodes[2] = { (huffman_node_t *)node1, (huffman_node_t *)node2 };
	
	return (int)nodes[0]->ascii - (int)nodes[1]->ascii;	
}

int huffman_compress(unsigned char *compr, unsigned long *comprlen, unsigned char *uncompr, unsigned long uncomprlen)
{
	/* n��Ҷ�ӵĹ�������Ҫ����n-1�κϲ�������n-1���½�㡣
	 * ������õĹ��������й���2n-1����㡣*/
	huffman_node_t nodes[2 * 256 - 1];	/* huffman�����������(2 ^ N - 1) */
	unsigned int leaves;
	unsigned int output_bits;
	unsigned long i;
	huffman_node_t *root;
	struct bits bits;
	
	memset(nodes, 0, sizeof(nodes));	

	/* ǰ256�����(N��������ֵ)���ڴ�Ź���������Ҷ��� */
	for (i = 0; i < 256; i++)
		nodes[i].ascii = (unit8)i;	/* for debug: ��Ǹ�Ҷ����������asciiֵ */
		
	/* ����������ֽ����ݵĳ���Ƶ�� */
	for (i = 0; i < uncomprlen; i++)
		nodes[uncompr[i]].weight++;
		
	/* ����Ƶ�ȣ�Ȩ���������� */
	qsort(nodes, 256, sizeof(huffman_node_t), huffman_weight_compare);
	
	/* ����huffman�� */	
	leaves = huffman_tree_create(nodes);

	root = &nodes[0];
	while (root->parent)
		root = root->parent;

	bits_init(&bits, compr, *comprlen);
	if (huffman_code_tree_encode(&bits, root))
		return -1;

	// sort nodes depending on ascii to can index nodes with its ascii value
	// �Ա������������
	qsort(nodes, 256, sizeof(huffman_node_t), huffman_ascii_compare);

	output_bits = bits.curbyte * 8 + bits.curbits;
	for (i = 0; i < uncomprlen; i++)
	{
		if (bits_put_high(&bits, nodes[uncompr[i]].code_lengths, (unsigned char *)&nodes[uncompr[i]].code))
				break;
		output_bits += nodes[uncompr[i]].code_lengths;
	}
	if (i != uncomprlen)
		return -1;
	*comprlen = ((output_bits + 8 - 1) & ~(8 - 1)) / 8;
	bits_flush(&bits);

	return 0;
}