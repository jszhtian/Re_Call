#include	"tonanet3_pch.h"
#include	"tonanet3_network.h"

namespace NT3
{
	C_network::C_network()
	{
		is_startup = false;
		is_listen = false;
		is_server = false;
		recv_data = NULL;

		m_h_wnd = NULL;
	}

	C_network::~C_network()
	{
		release();
	}

	// ****************************************************************
	// WinSock の初期化
	// ================================================================
	bool C_network::init(HWND h_wnd)
	{
		// WSA の初期化
		WSADATA wsa_data;
		if (WSAStartup(MAKEWORD(2, 0), &wsa_data) != 0)
			return false;

		is_startup = true;
		m_h_wnd = h_wnd;
		return true;
	}

	// ****************************************************************
	// WinSock の開放
	// ================================================================
	bool C_network::release()
	{
		// 受信データの開放
		if (recv_data)	{
			free(recv_data);
			recv_data = NULL;
		}

		// リスンソケットの開放
		if (is_listen)	{
			closesocket(listen_socket);
			is_listen = false;
		}

		// WinSock の開放
		if (is_startup)	{
			WSACleanup();
			is_startup = false;
		}

		m_h_wnd = NULL;
		return true;
	}

	// ****************************************************************
	// サーバになる
	// ================================================================
	bool C_network::create_server(int port)
	{
		if (!is_startup)
			return false;

		if (is_server)
			return false;

		// リスンソケットの準備
		listen_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (listen_socket == INVALID_SOCKET)
			return false;

		// リスンソケットの設定
		SOCKADDR_IN saddr;
		ZeroMemory(&saddr, sizeof(saddr));
		saddr.sin_family = AF_INET;
		saddr.sin_port = htons(port);
		saddr.sin_addr.s_addr = INADDR_ANY;

		// リスンソケットのバインド
		if (::bind(listen_socket, (sockaddr *)&saddr, sizeof(saddr)) == SOCKET_ERROR)	{
			closesocket(listen_socket);
			return false;
		}

		// リスン
		if (::listen(listen_socket, 0) == SOCKET_ERROR)	{
			closesocket(listen_socket);
			return false;
		}

		// イベントを受け取るウィンドウの設定
		if (WSAAsyncSelect(listen_socket, m_h_wnd, WM_SOCKET_SELECT, FD_ACCEPT) == SOCKET_ERROR)	{
			closesocket(listen_socket);
			return false;
		}

		is_server = true;
		is_listen = true;
		return true;
	}

	// ****************************************************************
	// サーバを閉じる
	// ================================================================
	bool C_network::close_server()
	{
		if (!is_startup)
			return false;

		if (is_server)	{
			// リスンソケットを閉じる
			WSAAsyncSelect(listen_socket, m_h_wnd, WM_SOCKET_SELECT, 0);
			closesocket(listen_socket);
			is_server = false;
		}

		return true;
	}

	// ****************************************************************
	// クライアントの待ちうけ
	// ================================================================
	bool C_network::accept(SOCKET* sock)
	{
		if (!is_startup)
			return false;

		if (!is_listen)
			return false;

		SOCKADDR_IN from;
		int from_len = sizeof(from);

		// クライアントの待ちうけ
		*sock = ::accept(listen_socket, (SOCKADDR *)&from, &from_len);
		if (*sock == INVALID_SOCKET)
			return false;

		// イベントを受け取るウィンドウの設定
		if (WSAAsyncSelect(*sock, m_h_wnd, WM_SOCKET_SELECT, FD_READ | FD_CLOSE) == SOCKET_ERROR)
			return false;

		return true;
	}

	// ****************************************************************
	// セッションを閉じる
	// ================================================================
	bool C_network::close(SOCKET sock)
	{
		if (!is_startup)
			return false;

		WSAAsyncSelect(sock, m_h_wnd, WM_SOCKET_SELECT, 0);
		shutdown(sock, SD_BOTH);	// シャットダウンしてから
		closesocket(sock);			// ソケットを閉じる

		return true;
	}

	// ****************************************************************
	// クライアントになる
	// ================================================================
	bool C_network::create_client(SOCKET* sock_, CTSTR& server_addr, int port)
	{
		if (!is_startup)
			return false;

		// ソケットの作成
		SOCKET sock = ::socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET)
			return false;

		// サーバアドレスの取得
		MBSTR server_addr_mb = TSTR_to_MBSTR(server_addr);
		HOSTENT* host = gethostbyname(server_addr_mb.c_str());

		// ホスト名じゃなくてホストアドレスだった場合の処理
		if (host == NULL)	{
			PTR addr = inet_addr(server_addr_mb.c_str());
			host = gethostbyaddr((char *)&addr, 4, AF_INET);		// IPv6 に未対応です＞＜
		}
		if (host == NULL)	{
			closesocket(sock);
			return false;
		}

		// ソケットの設定
		SOCKADDR_IN cs_in;
		ZeroMemory(&cs_in, sizeof(cs_in));
		cs_in.sin_family = host->h_addrtype;
		cs_in.sin_port = htons(port);
		cs_in.sin_addr.s_addr = *((u_long *)host->h_addr);
	
		// イベントを受け取るウィンドウの設定
		if (WSAAsyncSelect(sock, m_h_wnd, WM_SOCKET_SELECT, FD_CONNECT) == SOCKET_ERROR)	{
			closesocket(sock);
			return false;
		}

		// サーバに接続
		if (::connect(sock, (SOCKADDR *)&cs_in, sizeof(cs_in)) == SOCKET_ERROR)	{
			if (WSAGetLastError() != WSAEWOULDBLOCK)	{
				closesocket(sock);
				return false;
			}
		}

		*sock_ = sock;

		return true;
	}

	// ****************************************************************
	// サーバに接続
	// ================================================================
	bool C_network::connect(SOCKET sock)
	{
		if (!is_startup)
			return false;

		// イベントを受け取るウィンドウの設定
		if (WSAAsyncSelect(sock, m_h_wnd, WM_SOCKET_SELECT, FD_READ | FD_CLOSE) == SOCKET_ERROR)	{
			closesocket(sock);
			return false;
		}

		return true;
	}

	// ****************************************************************
	// データの受信
	// ================================================================
	bool C_network::recieve(SOCKET sock, BUFFER& data)
	{
		if (!is_startup)
			return false;

		int block_size = 1024;	// ブロックサイズ = 1kb

		for (int b = 0; ; b++)	{
			data.resize(block_size * (b + 1));
			int read_cnt = ::recv(sock, (char *)(data.get() + b * block_size), block_size, 0);
			if (read_cnt == SOCKET_ERROR)
				break;
		}

		return true;
	}

	// ****************************************************************
	// データの送信
	// ================================================================
	bool C_network::send(SOCKET sock, BYTE* data, int data_size)
	{
		if (!is_startup)
			return false;

		if (::send(sock, (char *)data, data_size, 0) == SOCKET_ERROR)
			return false;

		return true;
	}
}
