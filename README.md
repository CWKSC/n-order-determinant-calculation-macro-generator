# N-order determinant calculation macro generator

 N-order determinant calculation macro generator 
 
 N 阶行列式计算宏生成器
 
 N 階行列式計算巨集生成器

1. [【Just For Fun】n 階行列式計算 宏 生成器，四阶行列式的最优展开 - 知乎](https://zhuanlan.zhihu.com/p/77057988)
2. [【Just For Fun】n 階行列式計算 宏 生成器（重构） - 知乎](https://zhuanlan.zhihu.com/p/77388741)

### ▌n 阶行列式计算 宏 生成器（重构）：

```c
#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

#define ffor(var, start, end) \
    for(int var = start; var <= end; var++)

FILE* outputFile;

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
```

代码行数多了很多。

注解我不写了，我的代码应该能够自解释吧 (｡・ω・｡)

功能多了一点点，可以选择是否生成符合阵列使用的方式。

并且可以选择是否每一个都生成，还是只生成最后一个。

### ▌看看实际操作：

![img](https://pic2.zhimg.com/80/v2-3fe70f6edc333678a44c2714ced3988d_hd.png)

选择 n ，这里试试 10

![img](https://pic1.zhimg.com/80/v2-0fe2bf0cdeec51c04834a35491e1aa0c_hd.png)

是否生成符合阵列使用的方式 ？Yes

![img](https://pic4.zhimg.com/80/v2-3ef238590a37c521c0c9327a7c3be577_hd.png)

是否每一个都生成，还是只生成最后一个 ？每一个都生成。

这个程序会生成以下的文件：

```text
#define Det2_macro(a1, a2, a3, a4) ((a1) * (a4) - (a2) * (a3))
#define Det2_macro_ForArray(arr) Det2_macro((arr)[0], (arr)[1], (arr)[2], (arr)[3])
#define Det3_macro(a1, a2, a3, a4, a5, a6, a7, a8, a9) ( + (a1) * Det2_macro(a5, a6, a8, a9) - (a2) * Det2_macro(a4, a6, a7, a9) + (a3) * Det2_macro(a4, a5, a7, a8))
#define Det3_macro_ForArray(arr) Det3_macro((arr)[0], (arr)[1], (arr)[2], (arr)[3], (arr)[4], (arr)[5], (arr)[6], (arr)[7], (arr)[8])
#define Det4_macro(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16) ( + (a1) * Det3_macro(a6, a7, a8, a10, a11, a12, a14, a15, a16) - (a2) * Det3_macro(a5, a7, a8, a9, a11, a12, a13, a15, a16) + (a3) * Det3_macro(a5, a6, a8, a9, a10, a12, a13, a14, a16) - (a4) * Det3_macro(a5, a6, a7, a9, a10, a11, a13, a14, a15))
#define Det4_macro_ForArray(arr) Det4_macro((arr)[0], (arr)[1], (arr)[2], (arr)[3], (arr)[4], (arr)[5], (arr)[6], (arr)[7], (arr)[8], (arr)[9], (arr)[10], (arr)[11], (arr)[12], (arr)[13], (arr)[14], (arr)[15])
#define Det5_macro(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25) ( + (a1) * Det4_macro(a7, a8, a9, a10, a12, a13, a14, a15, a17, a18, a19, a20, a22, a23, a24, a25) - (a2) * Det4_macro(a6, a8, a9, a10, a11, a13, a14, a15, a16, a18, a19, a20, a21, a23, a24, a25) + (a3) * Det4_macro(a6, a7, a9, a10, a11, a12, a14, a15, a16, a17, a19, a20, a21, a22, a24, a25) - (a4) * Det4_macro(a6, a7, a8, a10, a11, a12, a13, a15, a16, a17, a18, a20, a21, a22, a23, a25) + (a5) * Det4_macro(a6, a7, a8, a9, a11, a12, a13, a14, a16, a17, a18, a19, a21, a22, a23, a24))
#define Det5_macro_ForArray(arr) Det5_macro((arr)[0], (arr)[1], (arr)[2], (arr)[3], (arr)[4], (arr)[5], (arr)[6], (arr)[7], (arr)[8], (arr)[9], (arr)[10], (arr)[11], (arr)[12], (arr)[13], (arr)[14], (arr)[15], (arr)[16], (arr)[17], (arr)[18], (arr)[19], (arr)[20], (arr)[21], (arr)[22], (arr)[23], (arr)[24])
#define Det6_macro(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36) ( + (a1) * Det5_macro(a8, a9, a10, a11, a12, a14, a15, a16, a17, a18, a20, a21, a22, a23, a24, a26, a27, a28, a29, a30, a32, a33, a34, a35, a36) - (a2) * Det5_macro(a7, a9, a10, a11, a12, a13, a15, a16, a17, a18, a19, a21, a22, a23, a24, a25, a27, a28, a29, a30, a31, a33, a34, a35, a36) + (a3) * Det5_macro(a7, a8, a10, a11, a12, a13, a14, a16, a17, a18, a19, a20, a22, a23, a24, a25, a26, a28, a29, a30, a31, a32, a34, a35, a36) - (a4) * Det5_macro(a7, a8, a9, a11, a12, a13, a14, a15, a17, a18, a19, a20, a21, a23, a24, a25, a26, a27, a29, a30, a31, a32, a33, a35, a36) + (a5) * Det5_macro(a7, a8, a9, a10, a12, a13, a14, a15, a16, a18, a19, a20, a21, a22, a24, a25, a26, a27, a28, a30, a31, a32, a33, a34, a36) - (a6) * Det5_macro(a7, a8, a9, a10, a11, a13, a14, a15, a16, a17, a19, a20, a21, a22, a23, a25, a26, a27, a28, a29, a31, a32, a33, a34, a35))
#define Det6_macro_ForArray(arr) Det6_macro((arr)[0], (arr)[1], (arr)[2], (arr)[3], (arr)[4], (arr)[5], (arr)[6], (arr)[7], (arr)[8], (arr)[9], (arr)[10], (arr)[11], (arr)[12], (arr)[13], (arr)[14], (arr)[15], (arr)[16], (arr)[17], (arr)[18], (arr)[19], (arr)[20], (arr)[21], (arr)[22], (arr)[23], (arr)[24], (arr)[25], (arr)[26], (arr)[27], (arr)[28], (arr)[29], (arr)[30], (arr)[31], (arr)[32], (arr)[33], (arr)[34], (arr)[35])
#define Det7_macro(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49) ( + (a1) * Det6_macro(a9, a10, a11, a12, a13, a14, a16, a17, a18, a19, a20, a21, a23, a24, a25, a26, a27, a28, a30, a31, a32, a33, a34, a35, a37, a38, a39, a40, a41, a42, a44, a45, a46, a47, a48, a49) - (a2) * Det6_macro(a8, a10, a11, a12, a13, a14, a15, a17, a18, a19, a20, a21, a22, a24, a25, a26, a27, a28, a29, a31, a32, a33, a34, a35, a36, a38, a39, a40, a41, a42, a43, a45, a46, a47, a48, a49) + (a3) * Det6_macro(a8, a9, a11, a12, a13, a14, a15, a16, a18, a19, a20, a21, a22, a23, a25, a26, a27, a28, a29, a30, a32, a33, a34, a35, a36, a37, a39, a40, a41, a42, a43, a44, a46, a47, a48, a49) - (a4) * Det6_macro(a8, a9, a10, a12, a13, a14, a15, a16, a17, a19, a20, a21, a22, a23, a24, a26, a27, a28, a29, a30, a31, a33, a34, a35, a36, a37, a38, a40, a41, a42, a43, a44, a45, a47, a48, a49) + (a5) * Det6_macro(a8, a9, a10, a11, a13, a14, a15, a16, a17, a18, a20, a21, a22, a23, a24, a25, a27, a28, a29, a30, a31, a32, a34, a35, a36, a37, a38, a39, a41, a42, a43, a44, a45, a46, a48, a49) - (a6) * Det6_macro(a8, a9, a10, a11, a12, a14, a15, a16, a17, a18, a19, a21, a22, a23, a24, a25, a26, a28, a29, a30, a31, a32, a33, a35, a36, a37, a38, a39, a40, a42, a43, a44, a45, a46, a47, a49) + (a7) * Det6_macro(a8, a9, a10, a11, a12, a13, a15, a16, a17, a18, a19, a20, a22, a23, a24, a25, a26, a27, a29, a30, a31, a32, a33, a34, a36, a37, a38, a39, a40, a41, a43, a44, a45, a46, a47, a48))
#define Det7_macro_ForArray(arr) Det7_macro((arr)[0], (arr)[1], (arr)[2], (arr)[3], (arr)[4], (arr)[5], (arr)[6], (arr)[7], (arr)[8], (arr)[9], (arr)[10], (arr)[11], (arr)[12], (arr)[13], (arr)[14], (arr)[15], (arr)[16], (arr)[17], (arr)[18], (arr)[19], (arr)[20], (arr)[21], (arr)[22], (arr)[23], (arr)[24], (arr)[25], (arr)[26], (arr)[27], (arr)[28], (arr)[29], (arr)[30], (arr)[31], (arr)[32], (arr)[33], (arr)[34], (arr)[35], (arr)[36], (arr)[37], (arr)[38], (arr)[39], (arr)[40], (arr)[41], (arr)[42], (arr)[43], (arr)[44], (arr)[45], (arr)[46], (arr)[47], (arr)[48])
#define Det8_macro(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63, a64) ( + (a1) * Det7_macro(a10, a11, a12, a13, a14, a15, a16, a18, a19, a20, a21, a22, a23, a24, a26, a27, a28, a29, a30, a31, a32, a34, a35, a36, a37, a38, a39, a40, a42, a43, a44, a45, a46, a47, a48, a50, a51, a52, a53, a54, a55, a56, a58, a59, a60, a61, a62, a63, a64) - (a2) * Det7_macro(a9, a11, a12, a13, a14, a15, a16, a17, a19, a20, a21, a22, a23, a24, a25, a27, a28, a29, a30, a31, a32, a33, a35, a36, a37, a38, a39, a40, a41, a43, a44, a45, a46, a47, a48, a49, a51, a52, a53, a54, a55, a56, a57, a59, a60, a61, a62, a63, a64) + (a3) * Det7_macro(a9, a10, a12, a13, a14, a15, a16, a17, a18, a20, a21, a22, a23, a24, a25, a26, a28, a29, a30, a31, a32, a33, a34, a36, a37, a38, a39, a40, a41, a42, a44, a45, a46, a47, a48, a49, a50, a52, a53, a54, a55, a56, a57, a58, a60, a61, a62, a63, a64) - (a4) * Det7_macro(a9, a10, a11, a13, a14, a15, a16, a17, a18, a19, a21, a22, a23, a24, a25, a26, a27, a29, a30, a31, a32, a33, a34, a35, a37, a38, a39, a40, a41, a42, a43, a45, a46, a47, a48, a49, a50, a51, a53, a54, a55, a56, a57, a58, a59, a61, a62, a63, a64) + (a5) * Det7_macro(a9, a10, a11, a12, a14, a15, a16, a17, a18, a19, a20, a22, a23, a24, a25, a26, a27, a28, a30, a31, a32, a33, a34, a35, a36, a38, a39, a40, a41, a42, a43, a44, a46, a47, a48, a49, a50, a51, a52, a54, a55, a56, a57, a58, a59, a60, a62, a63, a64) - (a6) * Det7_macro(a9, a10, a11, a12, a13, a15, a16, a17, a18, a19, a20, a21, a23, a24, a25, a26, a27, a28, a29, a31, a32, a33, a34, a35, a36, a37, a39, a40, a41, a42, a43, a44, a45, a47, a48, a49, a50, a51, a52, a53, a55, a56, a57, a58, a59, a60, a61, a63, a64) + (a7) * Det7_macro(a9, a10, a11, a12, a13, a14, a16, a17, a18, a19, a20, a21, a22, a24, a25, a26, a27, a28, a29, a30, a32, a33, a34, a35, a36, a37, a38, a40, a41, a42, a43, a44, a45, a46, a48, a49, a50, a51, a52, a53, a54, a56, a57, a58, a59, a60, a61, a62, a64) - (a8) * Det7_macro(a9, a10, a11, a12, a13, a14, a15, a17, a18, a19, a20, a21, a22, a23, a25, a26, a27, a28, a29, a30, a31, a33, a34, a35, a36, a37, a38, a39, a41, a42, a43, a44, a45, a46, a47, a49, a50, a51, a52, a53, a54, a55, a57, a58, a59, a60, a61, a62, a63))
#define Det8_macro_ForArray(arr) Det8_macro((arr)[0], (arr)[1], (arr)[2], (arr)[3], (arr)[4], (arr)[5], (arr)[6], (arr)[7], (arr)[8], (arr)[9], (arr)[10], (arr)[11], (arr)[12], (arr)[13], (arr)[14], (arr)[15], (arr)[16], (arr)[17], (arr)[18], (arr)[19], (arr)[20], (arr)[21], (arr)[22], (arr)[23], (arr)[24], (arr)[25], (arr)[26], (arr)[27], (arr)[28], (arr)[29], (arr)[30], (arr)[31], (arr)[32], (arr)[33], (arr)[34], (arr)[35], (arr)[36], (arr)[37], (arr)[38], (arr)[39], (arr)[40], (arr)[41], (arr)[42], (arr)[43], (arr)[44], (arr)[45], (arr)[46], (arr)[47], (arr)[48], (arr)[49], (arr)[50], (arr)[51], (arr)[52], (arr)[53], (arr)[54], (arr)[55], (arr)[56], (arr)[57], (arr)[58], (arr)[59], (arr)[60], (arr)[61], (arr)[62], (arr)[63])
#define Det9_macro(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81) ( + (a1) * Det8_macro(a11, a12, a13, a14, a15, a16, a17, a18, a20, a21, a22, a23, a24, a25, a26, a27, a29, a30, a31, a32, a33, a34, a35, a36, a38, a39, a40, a41, a42, a43, a44, a45, a47, a48, a49, a50, a51, a52, a53, a54, a56, a57, a58, a59, a60, a61, a62, a63, a65, a66, a67, a68, a69, a70, a71, a72, a74, a75, a76, a77, a78, a79, a80, a81) - (a2) * Det8_macro(a10, a12, a13, a14, a15, a16, a17, a18, a19, a21, a22, a23, a24, a25, a26, a27, a28, a30, a31, a32, a33, a34, a35, a36, a37, a39, a40, a41, a42, a43, a44, a45, a46, a48, a49, a50, a51, a52, a53, a54, a55, a57, a58, a59, a60, a61, a62, a63, a64, a66, a67, a68, a69, a70, a71, a72, a73, a75, a76, a77, a78, a79, a80, a81) + (a3) * Det8_macro(a10, a11, a13, a14, a15, a16, a17, a18, a19, a20, a22, a23, a24, a25, a26, a27, a28, a29, a31, a32, a33, a34, a35, a36, a37, a38, a40, a41, a42, a43, a44, a45, a46, a47, a49, a50, a51, a52, a53, a54, a55, a56, a58, a59, a60, a61, a62, a63, a64, a65, a67, a68, a69, a70, a71, a72, a73, a74, a76, a77, a78, a79, a80, a81) - (a4) * Det8_macro(a10, a11, a12, a14, a15, a16, a17, a18, a19, a20, a21, a23, a24, a25, a26, a27, a28, a29, a30, a32, a33, a34, a35, a36, a37, a38, a39, a41, a42, a43, a44, a45, a46, a47, a48, a50, a51, a52, a53, a54, a55, a56, a57, a59, a60, a61, a62, a63, a64, a65, a66, a68, a69, a70, a71, a72, a73, a74, a75, a77, a78, a79, a80, a81) + (a5) * Det8_macro(a10, a11, a12, a13, a15, a16, a17, a18, a19, a20, a21, a22, a24, a25, a26, a27, a28, a29, a30, a31, a33, a34, a35, a36, a37, a38, a39, a40, a42, a43, a44, a45, a46, a47, a48, a49, a51, a52, a53, a54, a55, a56, a57, a58, a60, a61, a62, a63, a64, a65, a66, a67, a69, a70, a71, a72, a73, a74, a75, a76, a78, a79, a80, a81) - (a6) * Det8_macro(a10, a11, a12, a13, a14, a16, a17, a18, a19, a20, a21, a22, a23, a25, a26, a27, a28, a29, a30, a31, a32, a34, a35, a36, a37, a38, a39, a40, a41, a43, a44, a45, a46, a47, a48, a49, a50, a52, a53, a54, a55, a56, a57, a58, a59, a61, a62, a63, a64, a65, a66, a67, a68, a70, a71, a72, a73, a74, a75, a76, a77, a79, a80, a81) + (a7) * Det8_macro(a10, a11, a12, a13, a14, a15, a17, a18, a19, a20, a21, a22, a23, a24, a26, a27, a28, a29, a30, a31, a32, a33, a35, a36, a37, a38, a39, a40, a41, a42, a44, a45, a46, a47, a48, a49, a50, a51, a53, a54, a55, a56, a57, a58, a59, a60, a62, a63, a64, a65, a66, a67, a68, a69, a71, a72, a73, a74, a75, a76, a77, a78, a80, a81) - (a8) * Det8_macro(a10, a11, a12, a13, a14, a15, a16, a18, a19, a20, a21, a22, a23, a24, a25, a27, a28, a29, a30, a31, a32, a33, a34, a36, a37, a38, a39, a40, a41, a42, a43, a45, a46, a47, a48, a49, a50, a51, a52, a54, a55, a56, a57, a58, a59, a60, a61, a63, a64, a65, a66, a67, a68, a69, a70, a72, a73, a74, a75, a76, a77, a78, a79, a81) + (a9) * Det8_macro(a10, a11, a12, a13, a14, a15, a16, a17, a19, a20, a21, a22, a23, a24, a25, a26, a28, a29, a30, a31, a32, a33, a34, a35, a37, a38, a39, a40, a41, a42, a43, a44, a46, a47, a48, a49, a50, a51, a52, a53, a55, a56, a57, a58, a59, a60, a61, a62, a64, a65, a66, a67, a68, a69, a70, a71, a73, a74, a75, a76, a77, a78, a79, a80))
#define Det9_macro_ForArray(arr) Det9_macro((arr)[0], (arr)[1], (arr)[2], (arr)[3], (arr)[4], (arr)[5], (arr)[6], (arr)[7], (arr)[8], (arr)[9], (arr)[10], (arr)[11], (arr)[12], (arr)[13], (arr)[14], (arr)[15], (arr)[16], (arr)[17], (arr)[18], (arr)[19], (arr)[20], (arr)[21], (arr)[22], (arr)[23], (arr)[24], (arr)[25], (arr)[26], (arr)[27], (arr)[28], (arr)[29], (arr)[30], (arr)[31], (arr)[32], (arr)[33], (arr)[34], (arr)[35], (arr)[36], (arr)[37], (arr)[38], (arr)[39], (arr)[40], (arr)[41], (arr)[42], (arr)[43], (arr)[44], (arr)[45], (arr)[46], (arr)[47], (arr)[48], (arr)[49], (arr)[50], (arr)[51], (arr)[52], (arr)[53], (arr)[54], (arr)[55], (arr)[56], (arr)[57], (arr)[58], (arr)[59], (arr)[60], (arr)[61], (arr)[62], (arr)[63], (arr)[64], (arr)[65], (arr)[66], (arr)[67], (arr)[68], (arr)[69], (arr)[70], (arr)[71], (arr)[72], (arr)[73], (arr)[74], (arr)[75], (arr)[76], (arr)[77], (arr)[78], (arr)[79], (arr)[80])
#define Det10_macro(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a90, a91, a92, a93, a94, a95, a96, a97, a98, a99, a100) ( + (a1) * Det9_macro(a12, a13, a14, a15, a16, a17, a18, a19, a20, a22, a23, a24, a25, a26, a27, a28, a29, a30, a32, a33, a34, a35, a36, a37, a38, a39, a40, a42, a43, a44, a45, a46, a47, a48, a49, a50, a52, a53, a54, a55, a56, a57, a58, a59, a60, a62, a63, a64, a65, a66, a67, a68, a69, a70, a72, a73, a74, a75, a76, a77, a78, a79, a80, a82, a83, a84, a85, a86, a87, a88, a89, a90, a92, a93, a94, a95, a96, a97, a98, a99, a100) - (a2) * Det9_macro(a11, a13, a14, a15, a16, a17, a18, a19, a20, a21, a23, a24, a25, a26, a27, a28, a29, a30, a31, a33, a34, a35, a36, a37, a38, a39, a40, a41, a43, a44, a45, a46, a47, a48, a49, a50, a51, a53, a54, a55, a56, a57, a58, a59, a60, a61, a63, a64, a65, a66, a67, a68, a69, a70, a71, a73, a74, a75, a76, a77, a78, a79, a80, a81, a83, a84, a85, a86, a87, a88, a89, a90, a91, a93, a94, a95, a96, a97, a98, a99, a100) + (a3) * Det9_macro(a11, a12, a14, a15, a16, a17, a18, a19, a20, a21, a22, a24, a25, a26, a27, a28, a29, a30, a31, a32, a34, a35, a36, a37, a38, a39, a40, a41, a42, a44, a45, a46, a47, a48, a49, a50, a51, a52, a54, a55, a56, a57, a58, a59, a60, a61, a62, a64, a65, a66, a67, a68, a69, a70, a71, a72, a74, a75, a76, a77, a78, a79, a80, a81, a82, a84, a85, a86, a87, a88, a89, a90, a91, a92, a94, a95, a96, a97, a98, a99, a100) - (a4) * Det9_macro(a11, a12, a13, a15, a16, a17, a18, a19, a20, a21, a22, a23, a25, a26, a27, a28, a29, a30, a31, a32, a33, a35, a36, a37, a38, a39, a40, a41, a42, a43, a45, a46, a47, a48, a49, a50, a51, a52, a53, a55, a56, a57, a58, a59, a60, a61, a62, a63, a65, a66, a67, a68, a69, a70, a71, a72, a73, a75, a76, a77, a78, a79, a80, a81, a82, a83, a85, a86, a87, a88, a89, a90, a91, a92, a93, a95, a96, a97, a98, a99, a100) + (a5) * Det9_macro(a11, a12, a13, a14, a16, a17, a18, a19, a20, a21, a22, a23, a24, a26, a27, a28, a29, a30, a31, a32, a33, a34, a36, a37, a38, a39, a40, a41, a42, a43, a44, a46, a47, a48, a49, a50, a51, a52, a53, a54, a56, a57, a58, a59, a60, a61, a62, a63, a64, a66, a67, a68, a69, a70, a71, a72, a73, a74, a76, a77, a78, a79, a80, a81, a82, a83, a84, a86, a87, a88, a89, a90, a91, a92, a93, a94, a96, a97, a98, a99, a100) - (a6) * Det9_macro(a11, a12, a13, a14, a15, a17, a18, a19, a20, a21, a22, a23, a24, a25, a27, a28, a29, a30, a31, a32, a33, a34, a35, a37, a38, a39, a40, a41, a42, a43, a44, a45, a47, a48, a49, a50, a51, a52, a53, a54, a55, a57, a58, a59, a60, a61, a62, a63, a64, a65, a67, a68, a69, a70, a71, a72, a73, a74, a75, a77, a78, a79, a80, a81, a82, a83, a84, a85, a87, a88, a89, a90, a91, a92, a93, a94, a95, a97, a98, a99, a100) + (a7) * Det9_macro(a11, a12, a13, a14, a15, a16, a18, a19, a20, a21, a22, a23, a24, a25, a26, a28, a29, a30, a31, a32, a33, a34, a35, a36, a38, a39, a40, a41, a42, a43, a44, a45, a46, a48, a49, a50, a51, a52, a53, a54, a55, a56, a58, a59, a60, a61, a62, a63, a64, a65, a66, a68, a69, a70, a71, a72, a73, a74, a75, a76, a78, a79, a80, a81, a82, a83, a84, a85, a86, a88, a89, a90, a91, a92, a93, a94, a95, a96, a98, a99, a100) - (a8) * Det9_macro(a11, a12, a13, a14, a15, a16, a17, a19, a20, a21, a22, a23, a24, a25, a26, a27, a29, a30, a31, a32, a33, a34, a35, a36, a37, a39, a40, a41, a42, a43, a44, a45, a46, a47, a49, a50, a51, a52, a53, a54, a55, a56, a57, a59, a60, a61, a62, a63, a64, a65, a66, a67, a69, a70, a71, a72, a73, a74, a75, a76, a77, a79, a80, a81, a82, a83, a84, a85, a86, a87, a89, a90, a91, a92, a93, a94, a95, a96, a97, a99, a100) + (a9) * Det9_macro(a11, a12, a13, a14, a15, a16, a17, a18, a20, a21, a22, a23, a24, a25, a26, a27, a28, a30, a31, a32, a33, a34, a35, a36, a37, a38, a40, a41, a42, a43, a44, a45, a46, a47, a48, a50, a51, a52, a53, a54, a55, a56, a57, a58, a60, a61, a62, a63, a64, a65, a66, a67, a68, a70, a71, a72, a73, a74, a75, a76, a77, a78, a80, a81, a82, a83, a84, a85, a86, a87, a88, a90, a91, a92, a93, a94, a95, a96, a97, a98, a100) - (a10) * Det9_macro(a11, a12, a13, a14, a15, a16, a17, a18, a19, a21, a22, a23, a24, a25, a26, a27, a28, a29, a31, a32, a33, a34, a35, a36, a37, a38, a39, a41, a42, a43, a44, a45, a46, a47, a48, a49, a51, a52, a53, a54, a55, a56, a57, a58, a59, a61, a62, a63, a64, a65, a66, a67, a68, a69, a71, a72, a73, a74, a75, a76, a77, a78, a79, a81, a82, a83, a84, a85, a86, a87, a88, a89, a91, a92, a93, a94, a95, a96, a97, a98, a99))
#define Det10_macro_ForArray(arr) Det10_macro((arr)[0], (arr)[1], (arr)[2], (arr)[3], (arr)[4], (arr)[5], (arr)[6], (arr)[7], (arr)[8], (arr)[9], (arr)[10], (arr)[11], (arr)[12], (arr)[13], (arr)[14], (arr)[15], (arr)[16], (arr)[17], (arr)[18], (arr)[19], (arr)[20], (arr)[21], (arr)[22], (arr)[23], (arr)[24], (arr)[25], (arr)[26], (arr)[27], (arr)[28], (arr)[29], (arr)[30], (arr)[31], (arr)[32], (arr)[33], (arr)[34], (arr)[35], (arr)[36], (arr)[37], (arr)[38], (arr)[39], (arr)[40], (arr)[41], (arr)[42], (arr)[43], (arr)[44], (arr)[45], (arr)[46], (arr)[47], (arr)[48], (arr)[49], (arr)[50], (arr)[51], (arr)[52], (arr)[53], (arr)[54], (arr)[55], (arr)[56], (arr)[57], (arr)[58], (arr)[59], (arr)[60], (arr)[61], (arr)[62], (arr)[63], (arr)[64], (arr)[65], (arr)[66], (arr)[67], (arr)[68], (arr)[69], (arr)[70], (arr)[71], (arr)[72], (arr)[73], (arr)[74], (arr)[75], (arr)[76], (arr)[77], (arr)[78], (arr)[79], (arr)[80], (arr)[81], (arr)[82], (arr)[83], (arr)[84], (arr)[85], (arr)[86], (arr)[87], (arr)[88], (arr)[89], (arr)[90], (arr)[91], (arr)[92], (arr)[93], (arr)[94], (arr)[95], (arr)[96], (arr)[97], (arr)[98], (arr)[99])
```

相对旧的代码，可输入的 n 放宽到 50。

经测试，n = 50 最大会生成 10 MB 的文件。

如有需要请自行修改源代码。

### ▌对生成出来的宏进行测试：

测试代码：

```c
#include <stdio.h>
#include <stdlib.h>

#define Det2_macro(a1, a2, a3, a4) ((a1) * (a4) - (a2) * (a3))
#define Det2_macro_ForArray(arr) Det2_macro((arr)[0], (arr)[1], (arr)[2], (arr)[3])
#define Det3_macro(a1, a2, a3, a4, a5, a6, a7, a8, a9) ( + (a1) * Det2_macro(a5, a6, a8, a9) - (a2) * Det2_macro(a4, a6, a7, a9) + (a3) * Det2_macro(a4, a5, a7, a8))
#define Det3_macro_ForArray(arr) Det3_macro((arr)[0], (arr)[1], (arr)[2], (arr)[3], (arr)[4], (arr)[5], (arr)[6], (arr)[7], (arr)[8])
#define Det4_macro(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16) ( + (a1) * Det3_macro(a6, a7, a8, a10, a11, a12, a14, a15, a16) - (a2) * Det3_macro(a5, a7, a8, a9, a11, a12, a13, a15, a16) + (a3) * Det3_macro(a5, a6, a8, a9, a10, a12, a13, a14, a16) - (a4) * Det3_macro(a5, a6, a7, a9, a10, a11, a13, a14, a15))
#define Det4_macro_ForArray(arr) Det4_macro((arr)[0], (arr)[1], (arr)[2], (arr)[3], (arr)[4], (arr)[5], (arr)[6], (arr)[7], (arr)[8], (arr)[9], (arr)[10], (arr)[11], (arr)[12], (arr)[13], (arr)[14], (arr)[15])
#define Det5_macro(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25) ( + (a1) * Det4_macro(a7, a8, a9, a10, a12, a13, a14, a15, a17, a18, a19, a20, a22, a23, a24, a25) - (a2) * Det4_macro(a6, a8, a9, a10, a11, a13, a14, a15, a16, a18, a19, a20, a21, a23, a24, a25) + (a3) * Det4_macro(a6, a7, a9, a10, a11, a12, a14, a15, a16, a17, a19, a20, a21, a22, a24, a25) - (a4) * Det4_macro(a6, a7, a8, a10, a11, a12, a13, a15, a16, a17, a18, a20, a21, a22, a23, a25) + (a5) * Det4_macro(a6, a7, a8, a9, a11, a12, a13, a14, a16, a17, a18, a19, a21, a22, a23, a24))
#define Det5_macro_ForArray(arr) Det5_macro((arr)[0], (arr)[1], (arr)[2], (arr)[3], (arr)[4], (arr)[5], (arr)[6], (arr)[7], (arr)[8], (arr)[9], (arr)[10], (arr)[11], (arr)[12], (arr)[13], (arr)[14], (arr)[15], (arr)[16], (arr)[17], (arr)[18], (arr)[19], (arr)[20], (arr)[21], (arr)[22], (arr)[23], (arr)[24])
#define Det6_macro(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36) ( + (a1) * Det5_macro(a8, a9, a10, a11, a12, a14, a15, a16, a17, a18, a20, a21, a22, a23, a24, a26, a27, a28, a29, a30, a32, a33, a34, a35, a36) - (a2) * Det5_macro(a7, a9, a10, a11, a12, a13, a15, a16, a17, a18, a19, a21, a22, a23, a24, a25, a27, a28, a29, a30, a31, a33, a34, a35, a36) + (a3) * Det5_macro(a7, a8, a10, a11, a12, a13, a14, a16, a17, a18, a19, a20, a22, a23, a24, a25, a26, a28, a29, a30, a31, a32, a34, a35, a36) - (a4) * Det5_macro(a7, a8, a9, a11, a12, a13, a14, a15, a17, a18, a19, a20, a21, a23, a24, a25, a26, a27, a29, a30, a31, a32, a33, a35, a36) + (a5) * Det5_macro(a7, a8, a9, a10, a12, a13, a14, a15, a16, a18, a19, a20, a21, a22, a24, a25, a26, a27, a28, a30, a31, a32, a33, a34, a36) - (a6) * Det5_macro(a7, a8, a9, a10, a11, a13, a14, a15, a16, a17, a19, a20, a21, a22, a23, a25, a26, a27, a28, a29, a31, a32, a33, a34, a35))
#define Det6_macro_ForArray(arr) Det6_macro((arr)[0], (arr)[1], (arr)[2], (arr)[3], (arr)[4], (arr)[5], (arr)[6], (arr)[7], (arr)[8], (arr)[9], (arr)[10], (arr)[11], (arr)[12], (arr)[13], (arr)[14], (arr)[15], (arr)[16], (arr)[17], (arr)[18], (arr)[19], (arr)[20], (arr)[21], (arr)[22], (arr)[23], (arr)[24], (arr)[25], (arr)[26], (arr)[27], (arr)[28], (arr)[29], (arr)[30], (arr)[31], (arr)[32], (arr)[33], (arr)[34], (arr)[35])
#define Det7_macro(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49) ( + (a1) * Det6_macro(a9, a10, a11, a12, a13, a14, a16, a17, a18, a19, a20, a21, a23, a24, a25, a26, a27, a28, a30, a31, a32, a33, a34, a35, a37, a38, a39, a40, a41, a42, a44, a45, a46, a47, a48, a49) - (a2) * Det6_macro(a8, a10, a11, a12, a13, a14, a15, a17, a18, a19, a20, a21, a22, a24, a25, a26, a27, a28, a29, a31, a32, a33, a34, a35, a36, a38, a39, a40, a41, a42, a43, a45, a46, a47, a48, a49) + (a3) * Det6_macro(a8, a9, a11, a12, a13, a14, a15, a16, a18, a19, a20, a21, a22, a23, a25, a26, a27, a28, a29, a30, a32, a33, a34, a35, a36, a37, a39, a40, a41, a42, a43, a44, a46, a47, a48, a49) - (a4) * Det6_macro(a8, a9, a10, a12, a13, a14, a15, a16, a17, a19, a20, a21, a22, a23, a24, a26, a27, a28, a29, a30, a31, a33, a34, a35, a36, a37, a38, a40, a41, a42, a43, a44, a45, a47, a48, a49) + (a5) * Det6_macro(a8, a9, a10, a11, a13, a14, a15, a16, a17, a18, a20, a21, a22, a23, a24, a25, a27, a28, a29, a30, a31, a32, a34, a35, a36, a37, a38, a39, a41, a42, a43, a44, a45, a46, a48, a49) - (a6) * Det6_macro(a8, a9, a10, a11, a12, a14, a15, a16, a17, a18, a19, a21, a22, a23, a24, a25, a26, a28, a29, a30, a31, a32, a33, a35, a36, a37, a38, a39, a40, a42, a43, a44, a45, a46, a47, a49) + (a7) * Det6_macro(a8, a9, a10, a11, a12, a13, a15, a16, a17, a18, a19, a20, a22, a23, a24, a25, a26, a27, a29, a30, a31, a32, a33, a34, a36, a37, a38, a39, a40, a41, a43, a44, a45, a46, a47, a48))
#define Det7_macro_ForArray(arr) Det7_macro((arr)[0], (arr)[1], (arr)[2], (arr)[3], (arr)[4], (arr)[5], (arr)[6], (arr)[7], (arr)[8], (arr)[9], (arr)[10], (arr)[11], (arr)[12], (arr)[13], (arr)[14], (arr)[15], (arr)[16], (arr)[17], (arr)[18], (arr)[19], (arr)[20], (arr)[21], (arr)[22], (arr)[23], (arr)[24], (arr)[25], (arr)[26], (arr)[27], (arr)[28], (arr)[29], (arr)[30], (arr)[31], (arr)[32], (arr)[33], (arr)[34], (arr)[35], (arr)[36], (arr)[37], (arr)[38], (arr)[39], (arr)[40], (arr)[41], (arr)[42], (arr)[43], (arr)[44], (arr)[45], (arr)[46], (arr)[47], (arr)[48])
#define Det8_macro(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63, a64) ( + (a1) * Det7_macro(a10, a11, a12, a13, a14, a15, a16, a18, a19, a20, a21, a22, a23, a24, a26, a27, a28, a29, a30, a31, a32, a34, a35, a36, a37, a38, a39, a40, a42, a43, a44, a45, a46, a47, a48, a50, a51, a52, a53, a54, a55, a56, a58, a59, a60, a61, a62, a63, a64) - (a2) * Det7_macro(a9, a11, a12, a13, a14, a15, a16, a17, a19, a20, a21, a22, a23, a24, a25, a27, a28, a29, a30, a31, a32, a33, a35, a36, a37, a38, a39, a40, a41, a43, a44, a45, a46, a47, a48, a49, a51, a52, a53, a54, a55, a56, a57, a59, a60, a61, a62, a63, a64) + (a3) * Det7_macro(a9, a10, a12, a13, a14, a15, a16, a17, a18, a20, a21, a22, a23, a24, a25, a26, a28, a29, a30, a31, a32, a33, a34, a36, a37, a38, a39, a40, a41, a42, a44, a45, a46, a47, a48, a49, a50, a52, a53, a54, a55, a56, a57, a58, a60, a61, a62, a63, a64) - (a4) * Det7_macro(a9, a10, a11, a13, a14, a15, a16, a17, a18, a19, a21, a22, a23, a24, a25, a26, a27, a29, a30, a31, a32, a33, a34, a35, a37, a38, a39, a40, a41, a42, a43, a45, a46, a47, a48, a49, a50, a51, a53, a54, a55, a56, a57, a58, a59, a61, a62, a63, a64) + (a5) * Det7_macro(a9, a10, a11, a12, a14, a15, a16, a17, a18, a19, a20, a22, a23, a24, a25, a26, a27, a28, a30, a31, a32, a33, a34, a35, a36, a38, a39, a40, a41, a42, a43, a44, a46, a47, a48, a49, a50, a51, a52, a54, a55, a56, a57, a58, a59, a60, a62, a63, a64) - (a6) * Det7_macro(a9, a10, a11, a12, a13, a15, a16, a17, a18, a19, a20, a21, a23, a24, a25, a26, a27, a28, a29, a31, a32, a33, a34, a35, a36, a37, a39, a40, a41, a42, a43, a44, a45, a47, a48, a49, a50, a51, a52, a53, a55, a56, a57, a58, a59, a60, a61, a63, a64) + (a7) * Det7_macro(a9, a10, a11, a12, a13, a14, a16, a17, a18, a19, a20, a21, a22, a24, a25, a26, a27, a28, a29, a30, a32, a33, a34, a35, a36, a37, a38, a40, a41, a42, a43, a44, a45, a46, a48, a49, a50, a51, a52, a53, a54, a56, a57, a58, a59, a60, a61, a62, a64) - (a8) * Det7_macro(a9, a10, a11, a12, a13, a14, a15, a17, a18, a19, a20, a21, a22, a23, a25, a26, a27, a28, a29, a30, a31, a33, a34, a35, a36, a37, a38, a39, a41, a42, a43, a44, a45, a46, a47, a49, a50, a51, a52, a53, a54, a55, a57, a58, a59, a60, a61, a62, a63))
#define Det8_macro_ForArray(arr) Det8_macro((arr)[0], (arr)[1], (arr)[2], (arr)[3], (arr)[4], (arr)[5], (arr)[6], (arr)[7], (arr)[8], (arr)[9], (arr)[10], (arr)[11], (arr)[12], (arr)[13], (arr)[14], (arr)[15], (arr)[16], (arr)[17], (arr)[18], (arr)[19], (arr)[20], (arr)[21], (arr)[22], (arr)[23], (arr)[24], (arr)[25], (arr)[26], (arr)[27], (arr)[28], (arr)[29], (arr)[30], (arr)[31], (arr)[32], (arr)[33], (arr)[34], (arr)[35], (arr)[36], (arr)[37], (arr)[38], (arr)[39], (arr)[40], (arr)[41], (arr)[42], (arr)[43], (arr)[44], (arr)[45], (arr)[46], (arr)[47], (arr)[48], (arr)[49], (arr)[50], (arr)[51], (arr)[52], (arr)[53], (arr)[54], (arr)[55], (arr)[56], (arr)[57], (arr)[58], (arr)[59], (arr)[60], (arr)[61], (arr)[62], (arr)[63])
#define Det9_macro(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81) ( + (a1) * Det8_macro(a11, a12, a13, a14, a15, a16, a17, a18, a20, a21, a22, a23, a24, a25, a26, a27, a29, a30, a31, a32, a33, a34, a35, a36, a38, a39, a40, a41, a42, a43, a44, a45, a47, a48, a49, a50, a51, a52, a53, a54, a56, a57, a58, a59, a60, a61, a62, a63, a65, a66, a67, a68, a69, a70, a71, a72, a74, a75, a76, a77, a78, a79, a80, a81) - (a2) * Det8_macro(a10, a12, a13, a14, a15, a16, a17, a18, a19, a21, a22, a23, a24, a25, a26, a27, a28, a30, a31, a32, a33, a34, a35, a36, a37, a39, a40, a41, a42, a43, a44, a45, a46, a48, a49, a50, a51, a52, a53, a54, a55, a57, a58, a59, a60, a61, a62, a63, a64, a66, a67, a68, a69, a70, a71, a72, a73, a75, a76, a77, a78, a79, a80, a81) + (a3) * Det8_macro(a10, a11, a13, a14, a15, a16, a17, a18, a19, a20, a22, a23, a24, a25, a26, a27, a28, a29, a31, a32, a33, a34, a35, a36, a37, a38, a40, a41, a42, a43, a44, a45, a46, a47, a49, a50, a51, a52, a53, a54, a55, a56, a58, a59, a60, a61, a62, a63, a64, a65, a67, a68, a69, a70, a71, a72, a73, a74, a76, a77, a78, a79, a80, a81) - (a4) * Det8_macro(a10, a11, a12, a14, a15, a16, a17, a18, a19, a20, a21, a23, a24, a25, a26, a27, a28, a29, a30, a32, a33, a34, a35, a36, a37, a38, a39, a41, a42, a43, a44, a45, a46, a47, a48, a50, a51, a52, a53, a54, a55, a56, a57, a59, a60, a61, a62, a63, a64, a65, a66, a68, a69, a70, a71, a72, a73, a74, a75, a77, a78, a79, a80, a81) + (a5) * Det8_macro(a10, a11, a12, a13, a15, a16, a17, a18, a19, a20, a21, a22, a24, a25, a26, a27, a28, a29, a30, a31, a33, a34, a35, a36, a37, a38, a39, a40, a42, a43, a44, a45, a46, a47, a48, a49, a51, a52, a53, a54, a55, a56, a57, a58, a60, a61, a62, a63, a64, a65, a66, a67, a69, a70, a71, a72, a73, a74, a75, a76, a78, a79, a80, a81) - (a6) * Det8_macro(a10, a11, a12, a13, a14, a16, a17, a18, a19, a20, a21, a22, a23, a25, a26, a27, a28, a29, a30, a31, a32, a34, a35, a36, a37, a38, a39, a40, a41, a43, a44, a45, a46, a47, a48, a49, a50, a52, a53, a54, a55, a56, a57, a58, a59, a61, a62, a63, a64, a65, a66, a67, a68, a70, a71, a72, a73, a74, a75, a76, a77, a79, a80, a81) + (a7) * Det8_macro(a10, a11, a12, a13, a14, a15, a17, a18, a19, a20, a21, a22, a23, a24, a26, a27, a28, a29, a30, a31, a32, a33, a35, a36, a37, a38, a39, a40, a41, a42, a44, a45, a46, a47, a48, a49, a50, a51, a53, a54, a55, a56, a57, a58, a59, a60, a62, a63, a64, a65, a66, a67, a68, a69, a71, a72, a73, a74, a75, a76, a77, a78, a80, a81) - (a8) * Det8_macro(a10, a11, a12, a13, a14, a15, a16, a18, a19, a20, a21, a22, a23, a24, a25, a27, a28, a29, a30, a31, a32, a33, a34, a36, a37, a38, a39, a40, a41, a42, a43, a45, a46, a47, a48, a49, a50, a51, a52, a54, a55, a56, a57, a58, a59, a60, a61, a63, a64, a65, a66, a67, a68, a69, a70, a72, a73, a74, a75, a76, a77, a78, a79, a81) + (a9) * Det8_macro(a10, a11, a12, a13, a14, a15, a16, a17, a19, a20, a21, a22, a23, a24, a25, a26, a28, a29, a30, a31, a32, a33, a34, a35, a37, a38, a39, a40, a41, a42, a43, a44, a46, a47, a48, a49, a50, a51, a52, a53, a55, a56, a57, a58, a59, a60, a61, a62, a64, a65, a66, a67, a68, a69, a70, a71, a73, a74, a75, a76, a77, a78, a79, a80))
#define Det9_macro_ForArray(arr) Det9_macro((arr)[0], (arr)[1], (arr)[2], (arr)[3], (arr)[4], (arr)[5], (arr)[6], (arr)[7], (arr)[8], (arr)[9], (arr)[10], (arr)[11], (arr)[12], (arr)[13], (arr)[14], (arr)[15], (arr)[16], (arr)[17], (arr)[18], (arr)[19], (arr)[20], (arr)[21], (arr)[22], (arr)[23], (arr)[24], (arr)[25], (arr)[26], (arr)[27], (arr)[28], (arr)[29], (arr)[30], (arr)[31], (arr)[32], (arr)[33], (arr)[34], (arr)[35], (arr)[36], (arr)[37], (arr)[38], (arr)[39], (arr)[40], (arr)[41], (arr)[42], (arr)[43], (arr)[44], (arr)[45], (arr)[46], (arr)[47], (arr)[48], (arr)[49], (arr)[50], (arr)[51], (arr)[52], (arr)[53], (arr)[54], (arr)[55], (arr)[56], (arr)[57], (arr)[58], (arr)[59], (arr)[60], (arr)[61], (arr)[62], (arr)[63], (arr)[64], (arr)[65], (arr)[66], (arr)[67], (arr)[68], (arr)[69], (arr)[70], (arr)[71], (arr)[72], (arr)[73], (arr)[74], (arr)[75], (arr)[76], (arr)[77], (arr)[78], (arr)[79], (arr)[80])
#define Det10_macro(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a90, a91, a92, a93, a94, a95, a96, a97, a98, a99, a100) ( + (a1) * Det9_macro(a12, a13, a14, a15, a16, a17, a18, a19, a20, a22, a23, a24, a25, a26, a27, a28, a29, a30, a32, a33, a34, a35, a36, a37, a38, a39, a40, a42, a43, a44, a45, a46, a47, a48, a49, a50, a52, a53, a54, a55, a56, a57, a58, a59, a60, a62, a63, a64, a65, a66, a67, a68, a69, a70, a72, a73, a74, a75, a76, a77, a78, a79, a80, a82, a83, a84, a85, a86, a87, a88, a89, a90, a92, a93, a94, a95, a96, a97, a98, a99, a100) - (a2) * Det9_macro(a11, a13, a14, a15, a16, a17, a18, a19, a20, a21, a23, a24, a25, a26, a27, a28, a29, a30, a31, a33, a34, a35, a36, a37, a38, a39, a40, a41, a43, a44, a45, a46, a47, a48, a49, a50, a51, a53, a54, a55, a56, a57, a58, a59, a60, a61, a63, a64, a65, a66, a67, a68, a69, a70, a71, a73, a74, a75, a76, a77, a78, a79, a80, a81, a83, a84, a85, a86, a87, a88, a89, a90, a91, a93, a94, a95, a96, a97, a98, a99, a100) + (a3) * Det9_macro(a11, a12, a14, a15, a16, a17, a18, a19, a20, a21, a22, a24, a25, a26, a27, a28, a29, a30, a31, a32, a34, a35, a36, a37, a38, a39, a40, a41, a42, a44, a45, a46, a47, a48, a49, a50, a51, a52, a54, a55, a56, a57, a58, a59, a60, a61, a62, a64, a65, a66, a67, a68, a69, a70, a71, a72, a74, a75, a76, a77, a78, a79, a80, a81, a82, a84, a85, a86, a87, a88, a89, a90, a91, a92, a94, a95, a96, a97, a98, a99, a100) - (a4) * Det9_macro(a11, a12, a13, a15, a16, a17, a18, a19, a20, a21, a22, a23, a25, a26, a27, a28, a29, a30, a31, a32, a33, a35, a36, a37, a38, a39, a40, a41, a42, a43, a45, a46, a47, a48, a49, a50, a51, a52, a53, a55, a56, a57, a58, a59, a60, a61, a62, a63, a65, a66, a67, a68, a69, a70, a71, a72, a73, a75, a76, a77, a78, a79, a80, a81, a82, a83, a85, a86, a87, a88, a89, a90, a91, a92, a93, a95, a96, a97, a98, a99, a100) + (a5) * Det9_macro(a11, a12, a13, a14, a16, a17, a18, a19, a20, a21, a22, a23, a24, a26, a27, a28, a29, a30, a31, a32, a33, a34, a36, a37, a38, a39, a40, a41, a42, a43, a44, a46, a47, a48, a49, a50, a51, a52, a53, a54, a56, a57, a58, a59, a60, a61, a62, a63, a64, a66, a67, a68, a69, a70, a71, a72, a73, a74, a76, a77, a78, a79, a80, a81, a82, a83, a84, a86, a87, a88, a89, a90, a91, a92, a93, a94, a96, a97, a98, a99, a100) - (a6) * Det9_macro(a11, a12, a13, a14, a15, a17, a18, a19, a20, a21, a22, a23, a24, a25, a27, a28, a29, a30, a31, a32, a33, a34, a35, a37, a38, a39, a40, a41, a42, a43, a44, a45, a47, a48, a49, a50, a51, a52, a53, a54, a55, a57, a58, a59, a60, a61, a62, a63, a64, a65, a67, a68, a69, a70, a71, a72, a73, a74, a75, a77, a78, a79, a80, a81, a82, a83, a84, a85, a87, a88, a89, a90, a91, a92, a93, a94, a95, a97, a98, a99, a100) + (a7) * Det9_macro(a11, a12, a13, a14, a15, a16, a18, a19, a20, a21, a22, a23, a24, a25, a26, a28, a29, a30, a31, a32, a33, a34, a35, a36, a38, a39, a40, a41, a42, a43, a44, a45, a46, a48, a49, a50, a51, a52, a53, a54, a55, a56, a58, a59, a60, a61, a62, a63, a64, a65, a66, a68, a69, a70, a71, a72, a73, a74, a75, a76, a78, a79, a80, a81, a82, a83, a84, a85, a86, a88, a89, a90, a91, a92, a93, a94, a95, a96, a98, a99, a100) - (a8) * Det9_macro(a11, a12, a13, a14, a15, a16, a17, a19, a20, a21, a22, a23, a24, a25, a26, a27, a29, a30, a31, a32, a33, a34, a35, a36, a37, a39, a40, a41, a42, a43, a44, a45, a46, a47, a49, a50, a51, a52, a53, a54, a55, a56, a57, a59, a60, a61, a62, a63, a64, a65, a66, a67, a69, a70, a71, a72, a73, a74, a75, a76, a77, a79, a80, a81, a82, a83, a84, a85, a86, a87, a89, a90, a91, a92, a93, a94, a95, a96, a97, a99, a100) + (a9) * Det9_macro(a11, a12, a13, a14, a15, a16, a17, a18, a20, a21, a22, a23, a24, a25, a26, a27, a28, a30, a31, a32, a33, a34, a35, a36, a37, a38, a40, a41, a42, a43, a44, a45, a46, a47, a48, a50, a51, a52, a53, a54, a55, a56, a57, a58, a60, a61, a62, a63, a64, a65, a66, a67, a68, a70, a71, a72, a73, a74, a75, a76, a77, a78, a80, a81, a82, a83, a84, a85, a86, a87, a88, a90, a91, a92, a93, a94, a95, a96, a97, a98, a100) - (a10) * Det9_macro(a11, a12, a13, a14, a15, a16, a17, a18, a19, a21, a22, a23, a24, a25, a26, a27, a28, a29, a31, a32, a33, a34, a35, a36, a37, a38, a39, a41, a42, a43, a44, a45, a46, a47, a48, a49, a51, a52, a53, a54, a55, a56, a57, a58, a59, a61, a62, a63, a64, a65, a66, a67, a68, a69, a71, a72, a73, a74, a75, a76, a77, a78, a79, a81, a82, a83, a84, a85, a86, a87, a88, a89, a91, a92, a93, a94, a95, a96, a97, a98, a99))
#define Det10_macro_ForArray(arr) Det10_macro((arr)[0], (arr)[1], (arr)[2], (arr)[3], (arr)[4], (arr)[5], (arr)[6], (arr)[7], (arr)[8], (arr)[9], (arr)[10], (arr)[11], (arr)[12], (arr)[13], (arr)[14], (arr)[15], (arr)[16], (arr)[17], (arr)[18], (arr)[19], (arr)[20], (arr)[21], (arr)[22], (arr)[23], (arr)[24], (arr)[25], (arr)[26], (arr)[27], (arr)[28], (arr)[29], (arr)[30], (arr)[31], (arr)[32], (arr)[33], (arr)[34], (arr)[35], (arr)[36], (arr)[37], (arr)[38], (arr)[39], (arr)[40], (arr)[41], (arr)[42], (arr)[43], (arr)[44], (arr)[45], (arr)[46], (arr)[47], (arr)[48], (arr)[49], (arr)[50], (arr)[51], (arr)[52], (arr)[53], (arr)[54], (arr)[55], (arr)[56], (arr)[57], (arr)[58], (arr)[59], (arr)[60], (arr)[61], (arr)[62], (arr)[63], (arr)[64], (arr)[65], (arr)[66], (arr)[67], (arr)[68], (arr)[69], (arr)[70], (arr)[71], (arr)[72], (arr)[73], (arr)[74], (arr)[75], (arr)[76], (arr)[77], (arr)[78], (arr)[79], (arr)[80], (arr)[81], (arr)[82], (arr)[83], (arr)[84], (arr)[85], (arr)[86], (arr)[87], (arr)[88], (arr)[89], (arr)[90], (arr)[91], (arr)[92], (arr)[93], (arr)[94], (arr)[95], (arr)[96], (arr)[97], (arr)[98], (arr)[99])

int main() {
	int arr[36] = { 23, 21, 62, 17, 82, 12,
                         3, 23, 61,  7, 71,  2,
                        41,  1,  2,  3,  4,  5,
                         6, 17, 87,  4,  6,  1,
                         3,  3,  5, 74,  7,  4,
                         4,  7,  1, 44,  2,  3 };
	printf("%d\n", Det6_macro_ForArray(arr));

	int arr2[49] = {  1,  3,  5,  7,  8,  2, 14,
	                 23,  2, 22,  7, 53, 14,  6, 
	                 41,  2,  6,  8,  4,  9,  3,
	                 16, 13, 36, 42,  5,  1,  7,
	                  2, 35, 12, 16, 17, 18,  2,
	                 26, 14,  8,  1, 18, 13,  3,
	                  6, 14,  7,  9,  6,  5,  9 };
	printf("%d\n", Det7_macro_ForArray(arr2));

	system("pause");
	return 0;
}
```

![img](https://pic2.zhimg.com/80/v2-6f84b5c8528a2742331d9bf50565da15_hd.png)

![img](https://pic4.zhimg.com/80/v2-2d59586d37712673ab7c3023f13ab4db_hd.jpg)

![img](https://pic4.zhimg.com/80/v2-e9009e3fdd4df9733acd366a9fc244b7_hd.jpg)

正确，完满结束~
