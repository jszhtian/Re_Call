#pragma		once

namespace NT3
{

	// ****************************************************************
	// ネットワーク
	// ================================================================
	class C_network
	{
	public:
		C_network();
		~C_network();

		bool	init(HWND h_wnd);
		bool	release();

		bool	create_server(int port);								// サーバになる
		bool	close_server();											// サーバを閉じる
		bool	accept(SOCKET* sock);									// クライアントの受付
		bool	close(SOCKET sock);										// セッションを閉じる
		bool	create_client(SOCKET* sock, CTSTR& server, int port);	// クライアントになる
		bool	connect(SOCKET sock);									// サーバに接続
		bool	recieve(SOCKET sock, BUFFER& data);						// データの受信
		bool	send(SOCKET sock, BYTE* pData, int dataSize);			// データの送信

	private:
		bool	is_startup;		// スタートアップフラグ
		bool	is_listen;		// リスンフラグ
		bool	is_server;		// サーバフラグ
		SOCKET	listen_socket;	// 待機ソケット
		BYTE*	recv_data;		// 受信データ

		HWND	m_h_wnd;			// メッセージを受け取るウィンドウ
	};

	// 唯一のインスタンス
	//extern CTonaNetwork		TheNetwork;
	//extern CTonaNetwork*	pNW;


}
