#include <bits/stdc++.h>
#include <string>
#include <algorithm>
#include <omp.h>
using namespace std;

int **DP_Seq;
int **DP_Parallel;
char* lcs_string;
int seq_lcs(string x, string y) 
{
	for(int i=0;i<=x.length();i++)
	{
		for(int j=0;j<=y.length();j++)
		{
			if(i==0||j==0)
			DP_Seq[i][j] = 0;

			else if(x[i-1]==y[j-1])
			DP_Seq[i][j]=DP_Seq[i-1][j-1]+1;

			else
			DP_Seq[i][j]=max(DP_Seq[i-1][j],DP_Seq[i][j-1]);
		}
	}
	return DP_Seq[x.length()][y.length()];
}

int parallel_lcs(string x,string y)
{
	for (int i=0,j=0; j<=y.length() && i<=x.length();j++)
	{

		int n=min(j,(int)x.length()-i);		//Size of the anti-diagonal
		#pragma omp parallel for
		for(int k=0;k<=n;++k)				//parallel for loop for each anti-diagonal element
		{
			int a=i+k,b=j-k;		        //for index of the diagonal elements 							
			if (a == 0 || b == 0)
				DP_Parallel[a][b] = 0;

			else if(x[a-1]==y[b-1])
				DP_Parallel[a][b]=DP_Parallel[a-1][b-1]+1;

			else
				DP_Parallel[a][b] = max(DP_Parallel[a-1][b],DP_Parallel[a][b-1]);
		}
		if(j>=y.length())
		{								
			j=y.length()-1,i++;
		}
	}
	return DP_Parallel[x.length()][y.length()];
}

void lcsString(string X, string Y)
{
	int index = DP_Seq[X.size()][Y.size()];
	lcs_string[index] = '\0'; 
	// we start from the right-most-bottom-most corner and one by one store characters in lcs_string[]
	int i = X.size(), j = Y.size();
	while (i > 0 && j > 0)
	{
		// If current character in X[] and Y are same, then current character is part of LCS
		if (X[i - 1] == Y[j - 1])
		{
			lcs_string[index - 1] = X[i - 1]; 
			i--; j--; index--;   
		}

		// If not same, then find the larger of two and go in the direction of larger value
		else if (DP_Seq[i - 1][j] > DP_Seq[i][j - 1])
			i--;
		else
			j--;
	}

}


int main()
{
	string string1, string2;
		int n = 0;
		printf("Enter length of string: ");
		cin >> n;

		DP_Seq = new int*[n+1];
		DP_Parallel = new int*[n+1];
		lcs_string= new char[n];

		for(int i=0;i<n+1;i++)
		{ 
			DP_Parallel[i] = new int[n+1];
			DP_Seq[i] = new int[n+1];
		}

		for (int i = 0; i < n; ++i)
			if (!(rand() % 4))
				string1 += " ";
			else
				string1 += rand() % 26 + 'a';

		int startPos = rand() % (n - 3);
		string2 = string1.substr(startPos, string1.size() - startPos - rand() % (startPos - 3));

		// cout << "string1  : " << string1 << endl;
		// cout << "string2  : " << string2 << endl;


	double t = omp_get_wtime();


	int lcs_parallel = parallel_lcs(string1, string2);
	cout << omp_get_wtime() - t << " sec computational time using parallel DP algorithm. " << endl;
	cout << "Longest Common Subsequence = " << lcs_parallel << "\n";

	t = omp_get_wtime();
	int lcs_seq = seq_lcs(string1, string2);
	cout << omp_get_wtime() - t << " sec computational time using sequential DP algorithm. " << endl;
	cout << "Longest Common Subsequence = " << lcs_seq << "\n"<<endl;
	

	return 0;
}
