#pragma		once

namespace NT3
{

	// ****************************************************************
	// �l�b�g���[�N
	// ================================================================
	class C_network
	{
	public:
		C_network();
		~C_network();

		bool	init(HWND h_wnd);
		bool	release();

		bool	create_server(int port);								// �T�[�o�ɂȂ�
		bool	close_server();											// �T�[�o�����
		bool	accept(SOCKET* sock);									// �N���C�A���g�̎�t
		bool	close(SOCKET sock);										// �Z�b�V���������
		bool	create_client(SOCKET* sock, CTSTR& server, int port);	// �N���C�A���g�ɂȂ�
		bool	connect(SOCKET sock);									// �T�[�o�ɐڑ�
		bool	recieve(SOCKET sock, BUFFER& data);						// �f�[�^�̎�M
		bool	send(SOCKET sock, BYTE* pData, int dataSize);			// �f�[�^�̑��M

	private:
		bool	is_startup;		// �X�^�[�g�A�b�v�t���O
		bool	is_listen;		// ���X���t���O
		bool	is_server;		// �T�[�o�t���O
		SOCKET	listen_socket;	// �ҋ@�\�P�b�g
		BYTE*	recv_data;		// ��M�f�[�^

		HWND	m_h_wnd;			// ���b�Z�[�W���󂯎��E�B���h�E
	};

	// �B��̃C���X�^���X
	//extern CTonaNetwork		TheNetwork;
	//extern CTonaNetwork*	pNW;


}
