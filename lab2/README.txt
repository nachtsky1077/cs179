
Question 1.1: Latency Hiding (5 points)
---------------------------------------

Approximately how many arithmetic instructions does it take to hide the latency
of a single arithmetic instruction on a GK110?

Assume all of the arithmetic instructions are independent (ie have no
instruction dependencies).

You do not need to consider the number of execution cores on the chip.

Hint: What is the latency of an arithmetic instruction? How many instructions
can a GK110 begin issuing in 1 clock cycle (assuming no dependencies)?
---------------------------------------
Answer:
The latency of an arithmetic instruction is 10ns. GK110 could begin issuing 8 instructions
in 1 clock cycle with 1GHz clock. Thus we need 10 * 8 = 80 instructions to hide the latency
of a single arithmetic instruction on a GK110.


Question 1.2: Thread Divergence (6 points)
------------------------------------------

Let the block shape be (32, 32, 1).

(a)
int idx = threadIdx.y + blockSize.y * threadIdx.x;
if (idx % 32 < 16)
    foo();
else
    bar();

Does this code diverge? Why or why not?

(b)
const float pi = 3.14;
float result = 1.0;
for (int i = 0; i < threadIdx.x; i++)
    result *= pi;

Does this code diverge? Why or why not? (This is a bit of a trick question,
either "yes" or "no can be a correct answer with appropriate explanation.)
------------------------------------------------

Answer:
(a) No. For block shape (32, 32, 1), we know each threadIdx.y has its own warp.
    For idx = threadIdx.y + blockSize.y * threadIdx.x, we have idx mod 32 = threadIdx.y,
    which means the same threadIdx.y will do the same task, hence the code does not diverge.

(b) Yes. Within the same warp (one same threadIdx.y), each thread do different number of 
    loop iterations.


Question 1.3: Coalesced Memory Access (9 points)
------------------------------------------------

Let the block shape be (32, 32, 1). Let data be a (float *) pointing to global
memory and let data be 128 byte aligned (so data % 128 == 0).

Consider each of the following access patterns.

(a)
data[threadIdx.x + blockSize.x * threadIdx.y] = 1.0;

Is this write coalesced? How many 128 byte cache lines does this write to?

(b)
data[threadIdx.y + blockSize.y * threadIdx.x] = 1.0;

Is this write coalesced? How many 128 byte cache lines does this write to?

(c)
data[1 + threadIdx.x + blockSize.x * threadIdx.y] = 1.0;

Is this write coalesced? How many 128 byte cache lines does this write to?
------------------------------------------------
Answer:
(a) Yes. 1 cache line.
(b) No. 32 cache lines.
(c) No. 2 cache lines.


Question 1.4: Bank Conflicts and Instruction Dependencies (15 points)
---------------------------------------------------------------------

Let's consider multiplying a 32 x 128 matrix with a 128 x 32 element matrix.
This outputs a 32 x 32 matrix. We'll use 32 ** 2 = 1024 threads and each thread
will compute 1 output element. Although its not optimal, for the sake of
simplicity let's use a single block, so grid shape = (1, 1, 1),
block shape = (32, 32, 1).

For the sake of this problem, let's assume both the left and right matrices have
already been stored in shared memory are in column major format. This means the
element in the ith row and jth column is accessible at lhs[i + 32 * j] for the
left hand side and rhs[i + 128 * j] for the right hand side.

This kernel will write to a variable called output stored in shared memory.

Consider the following kernel code:

int i = threadIdx.x;
int j = threadIdx.y;
for (int k = 0; k < 128; k += 2) {
    output[i + 32 * j] += lhs[i + 32 * k] * rhs[k + 128 * j];
    output[i + 32 * j] += lhs[i + 32 * (k + 1)] * rhs[(k + 1) + 128 * j];
}

(a)
Are there bank conflicts in this code? If so, how many ways is the bank conflict
(2-way, 4-way, etc)?

(b)
Expand the inner part of the loop (below)

output[i + 32 * j] += lhs[i + 32 * k] * rhs[k + 128 * j];
output[i + 32 * j] += lhs[i + 32 * (k + 1)] * rhs[(k + 1) + 128 * j];

into "psuedo-assembly" as was done in the coordinate addition example in lecture
4.

There's no need to expand the indexing math, only to expand the loads, stores,
and math. Notably, the operation a += b * c can be computed by a single
instruction called a fused multiply add (FMA), so this can be a single
instruction in your "psuedo-assembly".

Hint: Each line should expand to 5 instructions.

(c)
Identify pairs of dependent instructions in your answer to part b.

(d)
Rewrite the code given at the beginning of this problem to minimize instruction
dependencies. You can add or delete instructions (deleting an instruction is a
valid way to get rid of a dependency!) but each iteration of the loop must still
process 2 values of k.

(e)
Can you think of any other anything else you can do that might make this code
run faster?
---------------------------------------------------------------------
Answer:
(a) 










