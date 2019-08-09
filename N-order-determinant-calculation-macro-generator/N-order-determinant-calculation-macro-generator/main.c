#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

#define ffor(var, start, end) \
    for(int var = start; var <= end; var++)

FILE * outputFile;

void OpenFile() {
	if (fopen_s(&outputFile, "Det_macro.txt", "w") != 0) {
		puts("Can not open / create file.");
		exit(-1);
	}
}

void CloseFile() {
	if (outputFile == NULL) { return; }

	if (fclose(outputFile) == EOF) {
		puts("Can not close the file.");
		return;
	}
}

int upperLimitN;
int isItGenerateArrayMethod;
int isItGenerateArrayMethod_ForEach;

void QandA() {
	printf("n (3 <= n <= 50) ? : ");

	scanf_s("%d", &upperLimitN);
	if (!(3 <= upperLimitN && upperLimitN <= 50)) {
		printf("Out of limit, Modify the source code yourself as needed.");
		exit(-1);
	}

	printf("Generate Array Method (Yes = 1, No = 0) ? : ");
	scanf_s("%d", &isItGenerateArrayMethod);
	if (isItGenerateArrayMethod) {
		printf("Each or Only Last one (Each = 1, Only Last One = 0) ? : ");
		scanf_s("%d", &isItGenerateArrayMethod_ForEach);
	}
}

#define writeFile(str, ...) fprintf(outputFile, str, __VA_ARGS__)

void writeHeader(int n) {
	writeFile("#define Det%d_macro", n);
}

void writeParameterList(int n) {
	int parameterNum = n * n;
	writeFile("(");
	ffor(i, 1, parameterNum - 1) {
		writeFile("a%d, ", i);
	}
	writeFile("a%d) ", parameterNum);
}

int isItCofactorElement(int n, int cofactorParent, int element) {
	ffor(i, 1, n - 1) {
		if (element - i * n == cofactorParent) {
			return FALSE;
		}
	}
	return TRUE;
}

void writeBody_MarcoFunctionParameterList(int n, int cofactorParent) {
	writeFile("(");

	int firstPrint = TRUE;
#define firstElementOfSecondRow n + 1
	ffor(element, firstElementOfSecondRow, n * n) {

		if (isItCofactorElement(n, cofactorParent, element)) {
			if (firstPrint) {
				writeFile("a%d", element);
				firstPrint = FALSE;
			}
			else {
				writeFile(", a%d", element);
			}
		}

	}

	writeFile(")");
}

void writeBody(int n) {
	writeFile("(");

	int isItPos = TRUE;
	ffor(cofactorParent, 1, n) {

		writeFile(isItPos ? " + " : " - ");
		isItPos = isItPos ? FALSE : TRUE;

		writeFile("(a%d) * Det%d_macro", cofactorParent, n - 1);

		writeBody_MarcoFunctionParameterList(n, cofactorParent);

	}

	writeFile(")\n");
}

void writeArrayMethod(int n) {
	writeFile("#define Det%d_macro_ForArray(arr) Det%d_macro(", n, n);
	int parameterNum = n * n;
	ffor(i, 0, parameterNum - 2) {
		writeFile("(arr)[%d], ", i);
	}
	writeFile("(arr)[%d])\n", parameterNum - 1);
}

void GenerateFile() {
	writeFile("#define Det2_macro(a1, a2, a3, a4) ((a1) * (a4) - (a2) * (a3))\n");
	if (isItGenerateArrayMethod_ForEach) {
		writeFile("#define Det2_macro_ForArray(arr) Det2_macro((arr)[0], (arr)[1], (arr)[2], (arr)[3])\n");
	}

	ffor(n, 3, upperLimitN) {
		writeHeader(n);
		writeParameterList(n);
		writeBody(n);
		if (isItGenerateArrayMethod && isItGenerateArrayMethod_ForEach) {
			writeArrayMethod(n);
		}
	}
}

int main() {

	QandA();

	OpenFile();

	GenerateFile();

	CloseFile();

	return 0;
}
