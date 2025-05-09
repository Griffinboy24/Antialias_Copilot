



#if defined (_MSC_VER)
	#pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
	#pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
	#pragma warning (1 : 4705) // "statement has no effect"
	#pragma warning (1 : 4706) // "assignment within conditional expression"
	#pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
	#pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
	#pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include	"BaseVoiceState.h"
#include	"rspl.hpp"

#include	<cassert>
#include	<cmath>

namespace std {}



namespace rspl
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



BaseVoiceState::BaseVoiceState ()
:	_pos ()
,	_step ()
,	_table_ptr (0)
,	_table_len (0)
,	_table (0)
,	_ovrspl_flag (true)
{
	_pos._all  = 0;
	_step._all = static_cast <Int64> (0x80000000UL);
}



BaseVoiceState &	BaseVoiceState::operator = (const BaseVoiceState &other)
{
	assert (&other != 0);

	_pos         = other._pos;
	_step        = other._step;
	_table_ptr   = other._table_ptr;
	_table_len   = other._table_len;
	_table       = other._table;
	_ovrspl_flag = other._ovrspl_flag;

	return (*this);
}



void	BaseVoiceState::compute_step (long pitch)
{
	assert (_table >= 0);

	int				shift;

	if (pitch < 0)
	{
		// -1 => -1, -0x10000 => -1, -0x10001 => -2, etc
		shift = -1 - ((~pitch) >> NBR_BITS_PER_OCT);
	}
	else
	{
		shift = (pitch >> NBR_BITS_PER_OCT) - _table;
	}

	if (! _ovrspl_flag)
	{
		++ shift;
	}

	using namespace std;

	const int		mask = (1 << NBR_BITS_PER_OCT) - 1;
	const int		pitch_frac = static_cast <int> (pitch) & mask;
	_step._all = static_cast <Int64> (floor (
		exp (
			pitch_frac * (LN2 / static_cast <double> (1 << NBR_BITS_PER_OCT))
		) * (1UL << 31)
	));
	assert (_step._all >= static_cast <Int64> (1UL << 31));
	_step._all = shift_bidi (_step._all, shift);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}	// namespace rspl



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
