#include <stdio.h>
#include <stdlib.h>


/*************************************************************************
Given an input signal x[n], suppose we have two output signals y_1[n] and
y_2[n], given by the difference equations: 
		y_1[n] = x[n - 1] + x[n] + x[n + 1]
		y_2[n] = y_2[n - 2] + y_2[n - 1] + x[n]

Which calculation do you expect will have an easier and faster implementation on
the GPU, and why?

Answer:
The first one. Since the input and output of y_1[n] are two different arrays,
which means the input and output are independent, hence easy to parallelize.
y_2[n] is dependent on its previous values, which makes it hard to parallelize.
*************************************************************************/


/*************************************************************************
In class, we discussed how the exponential moving average (EMA), in comparison
to the simple moving average (SMA), is much less suited for parallelization on
the GPU. 

Recall that the EMA is given by:
	y[n] = c * x[n] + (1 - c) * y[n - 1]

Suppose that c is close to 1, and we only require an approximation to y[n]. How
can we get this approximation in a way that is parallelizable? (Explain in
words, optionally along with pseudocode or equations.)

Hint: If c is close to 1, then 1 - c is close to 0. If you expand the recurrence
relation a bit, what happens to the contribution (to y[n]) of the terms y[n - k]
as k increases?

Answer:
For EMA, we have:
y[n] = c * x[n] + (1 - c) * x[n - 1] + (1 - c)^2 * x[n - 2] + ... + (1 - c)^k * x[n - k] + ...
We could select a certain k and discard the terms (1 - c)^p * x[n - p] for any p > k.
We could do this since we ave c close to 1, which means (1 - c)^p is close to 0 if p is large.
In this way, we have the input and output independent which makes it easier for
us to parallelize the EMA.
*************************************************************************/
