#include "../includes/minishell.h"

static int	sig_code_static(int err_code, bool set_err_code)
{
	static int	static_err_code = 0;

	if (set_err_code)
		static_err_code = err_code;
	return (static_err_code);
}

void	set_sig_code(int err_code)
{
	sig_code_static(err_code, true);
}

int	get_sig_code(void)
{
	return (sig_code_static(0, false));
}