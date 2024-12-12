#pragma		once

namespace NT3
{

// ****************************************************************
// 汎用マネージャクラス
//		継承して、各種マネージャクラスを作成してください。
// ================================================================
template <typename RESOURCE, typename INFOSTRUCT>
class Cresource_manager
{
public:
	Cresource_manager() : keep_cnt(0)	{}
	~Cresource_manager()				{}

	struct KEEPER	{
		BSP<RESOURCE>	keep_s_ptr;		// 参照カウンタあり（keep_cnt >= 1)
		BWP<RESOURCE>	keep_w_ptr;		// 参照カウンタなし（keep_cnt == 0)
		int				keep_cnt;
	};

	typedef std::map< INFOSTRUCT, KEEPER >	MAP;
	typedef	std::pair< INFOSTRUCT, KEEPER >	PAIR;

	typename MAP::iterator	begin()	{	return resource_map.begin();	}
	typename MAP::iterator	end()	{	return resource_map.end();		}

	void set_keep_cnt(int cnt)
	{
		keep_cnt = cnt;
	}

	int	 get_resource_cnt()
	{
		return (int)resource_map.size();
	}

	// リソースを登録
	void regist(BSP<RESOURCE> resource, INFOSTRUCT* info)
	{
		// リソースをキープ
		KEEPER keeper;
		keeper.keep_cnt = keep_cnt;
		if (is_wp())	keeper.keep_w_ptr = resource;
		else			keeper.keep_s_ptr = resource;
		resource_map.insert(PAIR(*info, keeper));
	}

	// リソースの整理
	void reduce()
	{
		// 不要なリソースを削除
		for (MAP::iterator itr = resource_map.begin(); itr != resource_map.end(); )	{

			// wp 制御の場合
			if (is_wp())	{
				BSP<RESOURCE> resource = itr->second.keep_w_ptr.lock();
				if (!resource)		itr = resource_map.erase(itr);		// マップから削除
				else				++itr;								// 明示的に次の要素へ
			}

			// sp 制御の場合
			else	{
			
				// 参照カウントが１ → 外部から参照されていない
				if (itr->second.keep_s_ptr.use_count() == 1)	{
					// 保持カウントを減らす
					itr->second.keep_cnt --;
					// 保持カウントが０以下になったらリソースを解放
					if (itr->second.keep_cnt <= 0)	{
						itr->second.keep_s_ptr = BSP<RESOURCE>();	// 解放
						itr = resource_map.erase(itr);				// マップから削除
					}
					else	{
						++itr;	// 明示的に次の要素へ
					}
				}
				else	{
					++itr;	// 明示的に次の要素へ
				}
			}
		}
	}

	// リソースを検索
	BSP<RESOURCE> find(INFOSTRUCT* info)
	{
		// リソースが登録されているか検索
		MAP::iterator itr = resource_map.find(*info);

		// 見つかった場合
		if (itr != resource_map.end())	{
			// リソースを取得
			BSP<RESOURCE> resource = is_wp() ? itr->second.keep_w_ptr.lock() : itr->second.keep_s_ptr;
			// 有効なポインタならリソースを返す
			if (resource)	{
				itr->second.keep_cnt = keep_cnt;
				return resource;
			}
			// 無効なポインタならマップから削除
			else	{
				resource_map.erase(itr);
			}
		}

		// 無効なポインタを返す
		return BSP<RESOURCE>();
	}

protected:
	MAP		resource_map;
	int		keep_cnt;

	bool	is_wp()	{	return keep_cnt == 0;	}
	bool	is_sp()	{	return keep_cnt >= 1;	}
};

};
