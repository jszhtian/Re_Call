#pragma		once

namespace NT3
{

// ****************************************************************
// �ėp�}�l�[�W���N���X
//		�p�����āA�e��}�l�[�W���N���X���쐬���Ă��������B
// ================================================================
template <typename RESOURCE, typename INFOSTRUCT>
class Cresource_manager
{
public:
	Cresource_manager() : keep_cnt(0)	{}
	~Cresource_manager()				{}

	struct KEEPER	{
		BSP<RESOURCE>	keep_s_ptr;		// �Q�ƃJ�E���^����ikeep_cnt >= 1)
		BWP<RESOURCE>	keep_w_ptr;		// �Q�ƃJ�E���^�Ȃ��ikeep_cnt == 0)
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

	// ���\�[�X��o�^
	void regist(BSP<RESOURCE> resource, INFOSTRUCT* info)
	{
		// ���\�[�X���L�[�v
		KEEPER keeper;
		keeper.keep_cnt = keep_cnt;
		if (is_wp())	keeper.keep_w_ptr = resource;
		else			keeper.keep_s_ptr = resource;
		resource_map.insert(PAIR(*info, keeper));
	}

	// ���\�[�X�̐���
	void reduce()
	{
		// �s�v�ȃ��\�[�X���폜
		for (MAP::iterator itr = resource_map.begin(); itr != resource_map.end(); )	{

			// wp ����̏ꍇ
			if (is_wp())	{
				BSP<RESOURCE> resource = itr->second.keep_w_ptr.lock();
				if (!resource)		itr = resource_map.erase(itr);		// �}�b�v����폜
				else				++itr;								// �����I�Ɏ��̗v�f��
			}

			// sp ����̏ꍇ
			else	{
			
				// �Q�ƃJ�E���g���P �� �O������Q�Ƃ���Ă��Ȃ�
				if (itr->second.keep_s_ptr.use_count() == 1)	{
					// �ێ��J�E���g�����炷
					itr->second.keep_cnt --;
					// �ێ��J�E���g���O�ȉ��ɂȂ����烊�\�[�X�����
					if (itr->second.keep_cnt <= 0)	{
						itr->second.keep_s_ptr = BSP<RESOURCE>();	// ���
						itr = resource_map.erase(itr);				// �}�b�v����폜
					}
					else	{
						++itr;	// �����I�Ɏ��̗v�f��
					}
				}
				else	{
					++itr;	// �����I�Ɏ��̗v�f��
				}
			}
		}
	}

	// ���\�[�X������
	BSP<RESOURCE> find(INFOSTRUCT* info)
	{
		// ���\�[�X���o�^����Ă��邩����
		MAP::iterator itr = resource_map.find(*info);

		// ���������ꍇ
		if (itr != resource_map.end())	{
			// ���\�[�X���擾
			BSP<RESOURCE> resource = is_wp() ? itr->second.keep_w_ptr.lock() : itr->second.keep_s_ptr;
			// �L���ȃ|�C���^�Ȃ烊�\�[�X��Ԃ�
			if (resource)	{
				itr->second.keep_cnt = keep_cnt;
				return resource;
			}
			// �����ȃ|�C���^�Ȃ�}�b�v����폜
			else	{
				resource_map.erase(itr);
			}
		}

		// �����ȃ|�C���^��Ԃ�
		return BSP<RESOURCE>();
	}

protected:
	MAP		resource_map;
	int		keep_cnt;

	bool	is_wp()	{	return keep_cnt == 0;	}
	bool	is_sp()	{	return keep_cnt >= 1;	}
};

};
