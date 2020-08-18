#include <iostream>
#include <cstring>
#include <ctime>
#include <iomanip>

using namespace std;

//打印序列
void show(int a[], int size)
{
	for(int i = 0; i < size; ++i)
	{	
		cout << setw(3) << a[i] << " ";

		if((i+1) % 10 == 0)
			cout << endl;
	}

	cout << endl;
}

//计算排序算法花费的计算机时钟周期数（时间）
double count_time(int a[], int size, void (*func)(int a[], int size))
{
	time_t start, end;

	start = clock();
	func(a, size);
	end = clock();

	return difftime(end, start);
}

//直接插入排序
void try_insert(int a[], int size)
{
	for(int i = 1, j, tmp; i < size; i++)
	{	
		for(j = i, tmp = a[j]; j > 0 && tmp < a[j-1]; j--)
			a[j] = a[j-1];

		a[j] = tmp;
	}
}
//希尔排序
void try_shell(int a[], int size)
{
	for(int gap = size/2; gap > 0; gap /= 2)
		for(int i = gap, j, tmp; i < size; i++)
		{
			for(j = i, tmp = a[j]; j >= gap && tmp < a[j - gap]; j -= gap)
				a[j] = a[j - gap];

			a[j] = tmp;
		}
}

//基数排序
void try_radix(int a[], int size)
{
	//获取最大值
	int max = a[0];

	for(int i = 1; i < size; i++)
		if(max < a[i])
			max = a[i];

	//从个位开始按位排序，循环次数取决于最大元素的位数
	for(int exp = 1; max/exp > 0; exp *= 10)
	{
		int temp[size] = {0}, count[10] = {0};

		//遍历数组，并在count记录当前位的各个数字的个数(0~9)
		for(int i = 0; i < size; i++)
			count[a[i]/exp % 10]++;

		//在count记录每个数的位置
		for(int i = 1; i < 10; i++)
			count[i] += count[i-1];

		for(int i = size-1; i >= 0; i--)
		{
			//此时count存放的是个数，因为要作为下标所以减一
			temp[count[a[i]/exp % 10] - 1] = a[i];
			count[a[i]/exp % 10]--;
		}

		memcpy(a, temp, size*sizeof(int));
	}
}

//冒泡排序
void try_bubble(int a[], int size)
{
	for(int i = 0; i < size-1; i++)
		for(int j = 0, tmp; j < size-1-i; j++)
			if(a[j] > a[j+1])
			{
				tmp    = a[j];
				a[j]   = a[j+1];
				a[j+1] = tmp;
			}
}

//归并排序
void try_merge_unite(int a[], int left, int middle, int right)
{
	int i, j, k;
	int sizeL = middle-left + 1;
	int sizeR = right - middle;
	int L[sizeL] = {0}, R[sizeR] = {0};

	for(i = 0; i < sizeL; i++)
		L[i] = a[left + i];

	for(j = 0; j < sizeR; j++)
		R[j] = a[middle+1 + j];

	for(i = 0, j = 0, k = left; i < sizeL && j < sizeR; k++)
		if(L[i] <= R[j])
			a[k] = L[i++];
		else
			a[k] = R[j++];

	for(; i < sizeL; i++, k++)
		a[k] = L[i];

	for(; j < sizeR; j++, k++)
		a[k] = R[j];
}
void try_merge_split(int a[], int left, int right)
{
	if(left < right)
	{
		int middle = left + (right-left)/2;

		try_merge_split(a, left, middle);
		try_merge_split(a, middle+1, right);

		try_merge_unite(a, left, middle, right);
	}
}
void try_merge(int a[], int size)
{
	try_merge_split(a, 0, size-1);
}

//堆排序
void try_heapify(int a[], int root, int size)
{
	int largest = root;
	int left  = 2*root + 1;
	int right = 2*root + 2;

	//依次判断左右子树下标是否越界、寻找3者中最大值
	if(left  < size && a[left]  > a[largest])
		largest = left;
	if(right < size && a[right] > a[largest])
		largest = right;

	if(largest != root)
	{
		int tmp    = a[root];
		a[root]    = a[largest];
		a[largest] = tmp;

		//利用递归向后寻找，直到largest已为最后的子节点，则不会进入该判断语句
		try_heapify(a, largest, size);
	}
}
void try_heap(int a[], int size)
{	
	//堆化
	for(int i = size/2 - 1; i >= 0; i--)
		try_heapify(a, i, size);

	//从堆顶取出元素
	for(int i = size-1; i > 0; i--)
	{
		int tmp = a[0];
		a[0]    = a[i];
		a[i]    = tmp;

		try_heapify(a, 0, i);
	}
}

//直接选择排序
void try_select(int a[], int size)
{
	for(int i = 0, j, index; i < size; i++)
	{	
		for(j = i+1, index = i; j < size; j++)		
			if(a[index] > a[j])
				index = j;
				
		if(index != i)
		{
			int tmp  = a[i];
			a[i]     = a[index];
			a[index] = tmp;
		}
	}
}

//快速排序
void try_quick_sort(int a[], int left, int right)
{
	if(left >= right)
		return;
	
	int i = left, j = right, base = a[left];

	while(i < j)
	{
		while(a[j] >= base && i < j)
			j--;

		while(a[i] <= base && i < j)
			i++;

		if(i < j)
		{	
			int tmp = a[j];
			a[j]    = a[i];
			a[i]    = tmp;
		}
	}
	a[left] = a[j];
	a[j]	= base;

	try_quick_sort(a, left, j-1);
	try_quick_sort(a, j+1, right);
	
}
void try_quick(int a[], int size)
{
	//分治
	try_quick_sort(a, 0, size-1);
}

int main(int argc, char const *argv[])
{
	srand(time(NULL));

	int a[8][1000] = {0}; //选择排序、堆排序、插入排序、希尔排序、冒泡排序、快速排序、归并排序、基数排序

	for(int i = 0; i < 1000; i++)
		a[0][i] = rand() % 1000;

	for(int i = 1; i < 8; i++)
		memcpy(a[i], a[0], sizeof(a[0]));

	typedef void (*my_sort_t)(int [], int);
	my_sort_t result_sort[] = {try_select,
						 	   try_heap,
							   try_insert,
						 	   try_shell,
							   try_bubble,
							   try_quick,
						 	   try_merge,
							   try_radix};
                 
	char result_name[][32] = {"select: ",
							   "heap:   ",
							   "insert: ",
							   "shell:  ",
							   "bubble: ",
							   "quick:  ",
							   "merge:  ",
							   "radix:  "};
                 
	int size = sizeof(result_name)/sizeof(result_name[0]);

	double result_time[size] = {0};
	
	for(int i = 0; i < size; i++)
		result_time[i] = count_time(a[i], 1000, result_sort[i]);

	for(int i = 0, index; i < size; i++)
	{
		double max = -1;

		for(int j = 0; j < size; j++)
			if(result_time[j] > max)
			{	
				max = result_time[j];
				index = j;
			}

		cout << result_name[index] << result_time[index] << endl;

		result_time[index] = -1;
	}

	return 0;
}

