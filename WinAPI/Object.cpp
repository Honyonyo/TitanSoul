#include "stdafx.h"
#include "Object.h"

void Object::CoordSetting()
{
	m_tileLongitude = m_center.x / TILE_SIZE;
	 m_tileLatitude =  m_center.y / TILE_SIZE;

	 m_tileSpec = SCENEMANAGER->GetCurrentSceneTileMatSpec(m_tileLatitude, m_tileLongitude);
	 if (m_tileSpec > 4097) m_tileSpec -= 4097;
	 if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		 cout << m_tileSpec << endl;

}
