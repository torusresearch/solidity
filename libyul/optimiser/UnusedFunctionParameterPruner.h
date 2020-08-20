/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <libyul/optimiser/OptimiserStep.h>

namespace solidity::yul
{

/**
 * UnusedFunctionParameterPruner: Optimiser step that removes unused parameters in a function.
 *
 * If a parameter is unused, like `c` and `y` in, `function f(a,b,c) -> x, y { x := div(a,b) }`
 *
 * We remove the parameter and create a new "linking" function as follows:
 *
 * `function f(a,b) -> x { x := div(a,b) }`
 * `function f2(a,b,c) -> x, y { x := f(a,b) }`
 *
 * and replace all references to `f` by `f2`.
 *
 * Prerequisites: Disambiguator, SSATransform, ForLoopConditionIntoBody, ExpressionSplitter,
 * LiteralRematerialiser, FunctionHoister.
 *
 * The steps LiteralRematerialiser and SSATransform are not required for correctness.

 * LiteralRematerialiser helps deal with cases such as: `function f(x) -> y { revert(y, y} }` where
 * the literal `y` will be replaced by its value `0`, allowing us to rewrite the function.

 * The property "return variable is unused" is inferred by counting references to return variables.
 * SSATransform helps identifying that the return parameter is unused in the following case:
 *
 *    {
 *      let x := f()
 *      x := 1
 *    }
 *
 * The inliner should be run afterwards to make sure that all references to `f2` are replaced by
 * `f`.
 */
struct UnusedFunctionParameterPruner
{
	static constexpr char const* name{"UnusedFunctionParameterPruner"};
	static void run(OptimiserStepContext& _context, Block& _ast);
};

}