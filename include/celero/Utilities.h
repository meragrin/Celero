#ifndef H_CELERO_UTILITIES_H
#define H_CELERO_UTILITIES_H

// www.helleboreconsulting.com

///
/// \author	John Farrier
///
/// \copyright Copyright 2014 John Farrier 
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
/// 
/// http://www.apache.org/licenses/LICENSE-2.0
/// 
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///

#ifndef WIN32
#include <unistd.h>
#endif

#include <stdint.h>

namespace celero
{
	///
	/// \func DoNotOptimizeAway
	///
	/// Used to prevent compiler optimization of a variable
	/// that performs no real purpose other than to participate
	/// in a benchmark
	///
	/// Consider the following trivial benchmark:
	///
	/// \code
	/// BASELINE(...)
	/// {
	///     int x = 0;
	///
	///     for(int i = 0; i < 64; i++)
	///     {
	///         x += i;
	///     }
	/// }
	/// \endcode
	///
	/// Using Ubuntu clang v3.0, the resultant assembly is highly optimized
	/// as one might expect, but not terribly useful for baselining:
	///
	/// \verbatim
	/// movl	$2016, %eax             # imm = 0x7E0
	/// ret
	/// \endverbatim
	///
	/// Now, replace the inner loop with a call to DoNotOptimizeAway:
	///
	/// \code
	/// DoNotOptimizeAway(x += i);
	/// \endcode
	///
	/// The result is now a loop which is meaningful for establishing a 
	/// baseline.
	///
	/// \verbatim
	/// xorl	%ecx, %ecx
	/// xorl	%eax, %eax
	/// .LBB0_1:                                # =>This Inner Loop Header: Depth=1
	/// addl	%ecx, %eax
	/// incl	%ecx
	/// cmpl	$64, %ecx
	/// jne	.LBB0_1
	/// ret
	/// \endverbatim
	///
	/// GCC 4.8 gives similar results.
	///
	#ifdef WIN32
		template<class T> void DoNotOptimizeAway(T&& x) 
		{
			// Begin DoNotOptimizeAway 
			volatile static T* xPrime = &x;
			xPrime = &x;
			// End DoNotOptimizeAway
		}
	#else
		template<class T> void DoNotOptimizeAway(T&& x) 
		{
			// Begin DoNotOptimizeAway 
			asm volatile("" : "+r" (x));
			// End DoNotOptimizeAway
		}
	#endif

	///
	/// Quick definition of the number of microseconds per second.
	///
	const uint64_t UsPerSec(1000000);

	///
	/// Define the number of samples to default to for a good stastical sample when automatically timing tests.
	///
	const uint64_t StatisticalSample(30);
}

#endif
