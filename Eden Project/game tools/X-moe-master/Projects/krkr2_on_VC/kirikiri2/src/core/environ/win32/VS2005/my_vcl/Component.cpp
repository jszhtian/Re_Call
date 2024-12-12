#include "Component.h"
#ifndef ASSERT
#define ASSERT(x) assert(x)
#endif
//----------------------------------------------------------------------------
TComponent::TComponent(TComponent* owner)
{
	m_Owner = owner;
	if ( m_Owner )
		m_Owner->m_Components.push_back( this );
}


//----------------------------------------------------------------------------
TComponent::~TComponent()
{
	// �q������Ί�������
	DeleteChildren();

	// ���������ꂩ�̎q��������A����؂�
	std::vector<TComponent*>::iterator c;
	if ( m_Owner )
	{
		for(c=m_Owner->m_Components.begin(); c < m_Owner->m_Components.end();c++)
		{
			if ( *c == this )
			{
				m_Owner->m_Components.erase(c);
				break;
			}
		}
	}
}

//----------------------------------------------------------------------------
void TComponent::DeleteChildren()
{
	std::vector<TComponent*>::iterator c;

	// �q������Ί�������
	for(c=m_Components.begin(); c < m_Components.end(); c++)
	{
		TComponent* p = NULL;
		p = dynamic_cast<TComponent*>( *c );
		ASSERT(p);
		if ( p != NULL )
		{
			(*c)->m_Owner = NULL;
			delete *c;
		}
	}
	// �z�񎩑̂��N���A
	m_Components.clear();
}

//----------------------------------------------------------------------------
TComponent* TComponent::getComponents(int index) const
{
	return m_Components[index];
}

//----------------------------------------------------------------------------
int TComponent::getComponentCount() const
{
	return m_Components.size();
}
