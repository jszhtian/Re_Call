#pragma		once

#include	"tona3_string.h"

namespace NT3
{

void	valogo_create(CTSTR& check_filepath, int resource_id_logo_large, int resource_id_logo_small);
void	valogo_destroy();
HWND	valogo_get_handle();

}
