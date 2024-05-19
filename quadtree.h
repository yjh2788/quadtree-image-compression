#pragma once
#pragma warning(disable : 4996)
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "opencv2/opencv.hpp"
#include <stdlib.h>
#include <math.h>
#include <queue>
#include <stack>

#define Max_depth 10
using namespace cv;
using namespace std;
//const char* path = "C:\\Users\\spc\\Desktop\\Untitled Folder\\capture.jpg";
CString path;//= "C:\\Users\\spc\\Desktop\\Untitled Folder\\capture.jpg";

queue<bool> code;
stack<bool> tmp;
int minimum_block_size;
int cord_x = 0, cord_y = 0;

/// //////////////////////
struct Image {
public:

	int w;
	int h;
	Mat M_img;

	Image();
	void ImageFree(Image* img);
	float calcImageStd();
	void ImageRead(Image* img);
	int calcImageMean();
};
Image::Image()
{
	w = 0;
	h = 0;
}
void Image::ImageFree(Image* img)
{
	delete(img);
}
int Image::calcImageMean()
{
	w = this->M_img.cols;
	h = this->M_img.rows;

	int mean = 0;
	Mat rgb[3];
	split(this->M_img, rgb);
	Mat total = rgb[0] + rgb[1] + rgb[2];

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			mean = mean + total.at<uchar>(i, j);
		}
	}
	mean = mean / (w * h);
	return mean;
}
float Image::calcImageStd()
{
	Size s = size(this->M_img);
	int w = s.width;
	int h = s.height;
	Mat bands[3], total(h, w, CV_32SC1);
	split(this->M_img, bands);

	total = bands[0] + bands[1] + bands[2];///rgb다 더한거

	int mean = calcImageMean();

	int tmp, r, g, b;
	unsigned int Sum = 0;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			Sum = Sum + pow((total.at<uchar>(i, j) - mean), 2);
		}
	}
	float deviation = sqrt(Sum / (w * h));

	return deviation;
}
void Image::ImageRead(Image* img)
{
	FILE* fp = fopen(path, "rb");
	fclose(fp);
	this->M_img.create(img->h, img->w, CV_32SC1);
	const char* str = (CStringA)path;
	this->M_img = imread(str, 1);
}
/////////////////////////////////////////////////////

struct quadtreeNode {
public:
	Image* pic;
	bool split;
	int mean;
	int mean_r;
	int mean_g;
	int mean_b;
	quadtreeNode* L_Top;
	quadtreeNode* R_Top;
	quadtreeNode* L_Bot;
	quadtreeNode* R_Bot;
	quadtreeNode();
};
quadtreeNode::quadtreeNode()
{
	pic = NULL;
	split = 1;
	mean = 0;
	mean_r = 0;
	mean_g = 0;
	mean_b = 0;
	L_Top = NULL;
	R_Top = NULL;
	L_Bot = NULL;
	R_Bot = NULL;
}
struct quadtree_h {
	struct quadtreeNode* root;
};
class quadtree {
public:

	int depth;
	quadtree_h head_r;
	float total_sigma;
	quadtree();
	~quadtree();
	void free_quadtree();
	void img_init(Image* img);
	void quadrant(quadtreeNode* node);
	void encode(quadtreeNode* node);
	Mat* decode(unsigned char* data);
	void BFSearch(quadtreeNode* root);
	void saveAsFile( int sx, int sy, FILE* fp);

};

quadtree::quadtree()
{
	total_sigma = 15;
	depth = 1;
	head_r.root = new quadtreeNode;
	head_r.root->L_Bot = NULL;
	head_r.root->L_Top = NULL;
	head_r.root->R_Bot = NULL;
	head_r.root->R_Top = NULL;
}
quadtree::~quadtree()
{

}
void quadtree::free_quadtree()
{
	quadtreeNode* tmp;
	queue<quadtreeNode*> BFS;
	if (head_r.root->split == 1) encode(head_r.root);
	else
	{
		encode(head_r.root);
		BFS.push(head_r.root->L_Top);
		BFS.push(head_r.root->R_Top);
		BFS.push(head_r.root->L_Bot);
		BFS.push(head_r.root->R_Bot);
	}
	while (!(BFS.empty()))
	{
		tmp = BFS.front();
		delete(BFS.front()->pic);
		BFS.pop();
		encode(tmp);
		if (tmp->L_Top != NULL) BFS.push(tmp->L_Top);
		if (tmp->R_Top != NULL) BFS.push(tmp->R_Top);
		if (tmp->L_Bot != NULL) BFS.push(tmp->L_Bot);
		if (tmp->R_Bot != NULL) BFS.push(tmp->R_Bot);

	}
}///////////////////////////////만들기
void quadtree::img_init(Image* img)
{
	head_r.root->pic = img;
	head_r.root->pic->w = img->M_img.cols;
	head_r.root->pic->h = img->M_img.rows;
}
void quadtree::quadrant(quadtreeNode* node)
{
	
	float sigma;
	Mat img = node->pic->M_img;
	Size s = size(img);
	int w = s.width;
	int h = s.height;
	int mid_w = w / 2;
	int mid_h = h / 2;

	Mat rgb[3];
	split(node->pic->M_img, rgb);

	sigma = node->pic->calcImageStd();
	node->mean_b = mean(rgb[0])[0];
	node->mean_g = mean(rgb[1])[0];
	node->mean_r = mean(rgb[2])[0];

	if ((depth == Max_depth)||(w==1)||(h==1)||(w%2==1)||(h%2==1))	return;

	if (sigma > total_sigma)
	{
		depth++;
		node->split = false;
		quadtreeNode* LB = new quadtreeNode;
		quadtreeNode* LT = new quadtreeNode;
		quadtreeNode* RB = new quadtreeNode;
		quadtreeNode* RT = new quadtreeNode;
		LT->pic = new Image;
		LB->pic = new Image;
		RT->pic = new Image;
		RB->pic = new Image;

		LT->pic->M_img = img(Rect(0, 0, mid_w, mid_h));
		LB->pic->M_img = img(Rect(0,  mid_h, mid_w, mid_h));
		RT->pic->M_img = img(Rect(mid_w, 0, mid_w, mid_h));
		RB->pic->M_img = img(Rect(mid_w, mid_h, mid_w, mid_h));

		LT->mean = LT->pic->calcImageMean();
		LB->mean = LB->pic->calcImageMean();
		RT->mean = RT->pic->calcImageMean();
		RB->mean = RB->pic->calcImageMean();

		node->L_Bot = LB;
		node->R_Bot = RB;
		node->L_Top = LT;
		node->R_Top = RT;

		//imshow("img", node->L_Top->pic->M_img);
		//imshow("img1", node->R_Top->pic->M_img);
		//imshow("img2", node->L_Bot->pic->M_img);
		//imshow("img3", node->R_Bot->pic->M_img);
		//waitKey();
		//destroyAllWindows();	
	}
	else
	{
		node->split = true;
		return;
	}
	quadrant(node->L_Top);
	quadrant(node->R_Top);
	quadrant(node->L_Bot);
	quadrant(node->R_Bot);
	depth--;
}
void quadtree::encode(quadtreeNode* node)
{
	int m = node->mean;
	unsigned char rgb_mean[3];
	rgb_mean[0] = node->mean_r;
	rgb_mean[1] = node->mean_g;
	rgb_mean[2] = node->mean_b;
	bool r;
	if (!(node->split))
	{
		code.push(node->split);
	}
	else
	{
		code.push(node->split);
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				r = rgb_mean[i] % 2;
				tmp.push(r);
				rgb_mean[i] = rgb_mean[i] / 2;
			}
			while (!(tmp.empty()))
			{
				code.push(tmp.top());
				tmp.pop();
			}
		}
	}
}
Mat* quadtree::decode(unsigned char* data)/////데이터 읽어서 쿼드트리 생성
{
	queue<bool> bitstream;
	queue<Rect*> BFS;
	bool tmp;
	unsigned char mean_r = 0, mean_g = 0, mean_b = 0;
	int w = (data[3] << 8) + data[2];
	int h = (data[5] << 8) + data[4];
	int mid_w = w;
	int mid_h = h;
	Mat* compressed = new Mat(h, w, CV_32SC1);

	*compressed = head_r.root->pic->M_img.clone();
	///
	//bitstream 큐에 읽은 데이터 비트단위로 저장
	int k = _msize(data);


	for (int i = 6; i < k - 2; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			tmp = (data[i] & (char)pow(2, 7 - j)) >> 7 - j;
			bitstream.push(tmp);
		}
	}

	int j = data[k - 1];
	if (j != 0) j = 8 - j;
	for (j; j < 8; j++)
	{
		tmp = (data[k - 2] & (char)pow(2, 7 - j)) >> 7 - j;
		bitstream.push(tmp);
	}
	
	/// 디코딩
	BFS.push(new Rect(0, 0, w, h));
	while (!(bitstream.empty()))
	{
		tmp = bitstream.front();
		bitstream.pop();

		//1이면 8*3개 뽑기(안쪼개지는 부분)
		if (tmp == 1)
		{

			for (int i = 0; i < 8; i++)
			{
				if (bitstream.front() == 1) mean_r = (mean_r << 1) + 1;
				else mean_r = mean_r << 1;
				bitstream.pop();
			}
			for (int i = 0; i < 8; i++)
			{
				if (bitstream.front() == 1) mean_g = (mean_g << 1) + 1;
				else mean_g = mean_g << 1;
				bitstream.pop();
			}
			for (int i = 0; i < 8; i++)
			{
				if (bitstream.front() == 1) mean_b = (mean_b << 1) + 1;
				else mean_b = mean_b << 1;
				bitstream.pop();
			}
			(*compressed)(*(BFS.front())) = Scalar(mean_b, mean_g, mean_r);

			delete(BFS.front());
			BFS.pop();
		}
		else///0이면
		{
			mid_w = BFS.front()->width / 2;
			mid_h = BFS.front()->height / 2;
			for (int i = 0; i < 4; i++)
			{
				if (i == 0) BFS.push(new Rect(BFS.front()->x, BFS.front()->y, mid_w, mid_h));
				if (i == 1) BFS.push(new Rect(BFS.front()->x + mid_w, BFS.front()->y, mid_w, mid_h));
				if (i == 2) BFS.push(new Rect(BFS.front()->x, BFS.front()->y + mid_h, mid_w, mid_h));
				if (i == 3) BFS.push(new Rect(BFS.front()->x + mid_w, BFS.front()->y + mid_h, mid_w, mid_h));
			}
			delete(BFS.front());
			BFS.pop();
		}
	}

	minimum_block_size = mid_w * mid_h;
	return compressed;
}
void quadtree::BFSearch(quadtreeNode* root)
{
	quadtreeNode* tmp;
	queue<quadtreeNode*> BFS;
	if (root->split == 1) encode(root);
	else
	{
		encode(root);
		BFS.push(root->L_Top);
		BFS.push(root->R_Top);
		BFS.push(root->L_Bot);
		BFS.push(root->R_Bot);
	}
	while (!(BFS.empty()))
	{
		tmp = BFS.front();
		BFS.pop();
		encode(tmp);
		if (tmp->L_Top != NULL) BFS.push(tmp->L_Top);
		if (tmp->R_Top != NULL) BFS.push(tmp->R_Top);
		if (tmp->L_Bot != NULL) BFS.push(tmp->L_Bot);
		if (tmp->R_Bot != NULL) BFS.push(tmp->R_Bot);
	}
}
void quadtree::saveAsFile(int sx, int sy, FILE* fp)
{
	unsigned short size[2] = { sx,sy };
	char magic[2] = { 'K','Q' };
	fwrite(magic, sizeof(char), 2, fp);
	fwrite(size, sizeof(unsigned short), 2, fp);
	unsigned short num = 0;
	unsigned char count = 0;////
	while (!(code.empty()))
	{
		if (num == 8)////
		{
			num = 0;
			fwrite(&count, sizeof(char), 1, fp);/////
			count = 0;
		}
		if (code.front() == 1) count = (count << 1) + 1;
		else count = count << 1;
		//printf("%d", code.front());
		code.pop();
		num++;
	}
	if (num!= 0)
	{
		fwrite(&count, 1, 1, fp);
		fwrite(&num, 1, 1, fp);
	}
	else fwrite(&num, 1, 1, fp);

	fclose(fp);
}
unsigned char* readFile(FILE* fp)
{
	fseek(fp, 0, SEEK_SET);
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	unsigned char* str = new unsigned char[size];
	fseek(fp, 0, SEEK_SET);
	fread(str, sizeof(char), size, fp);
	fclose(fp);
	if ((str[0] == 'K') && (str[1] == 'Q')) return str;
	else return 0;
}
float PSNR(Mat a, Mat b)
{
	int sum = 0;
	int w = a.cols;
	int h = a.rows;
	a.convertTo(a, CV_8UC1);
	b.convertTo(b, CV_8UC1);
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			sum = sum + pow((a.at<uchar>(i, j)-b.at<uchar>(i,j)),2);
		}
	}
	float MSE = sum / (w * h);
	float PSNR = 10 * log10((255 * 255) / MSE);
	return PSNR;
}
