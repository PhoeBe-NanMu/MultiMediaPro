int main(int argc, char** argv)
{
	WIN32_FIND_DATAA FileData;
	HANDLE hFind;

	hFind = FindFirstFileA((LPCSTR)"Imgs/*.jpg", &FileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf("Invalid File Handle. GetLastError reports %d\n", GetLastError());
		return 0;
	}
	while (FindNextFileA(hFind, &FileData))
	{
		cout << FileData.cFileName << endl;
		string name("Imgs/");
		name.append(FileData.cFileName);
		IplImage* src = cvLoadImage(name.c_str());
		if (!src)
		{
			cout << "failed to load image" << endl;  //API
			//AfxMessageBox("failed to load image"); //MFC
			break;
		}
		assert(src->nChannels == 3);
		// ¥¶¿Ì
		IplImage* srcGray = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
		cvCvtColor(src, srcGray, CV_BGR2GRAY);
		// ±£¥Ê
		name.append(".srcGray.jpg");
		cvSaveImage(name.c_str(), srcGray);
		cvReleaseImage(&src);
		cvReleaseImage(&srcGray);
	}

	FindClose(&hFind);

	return 0;
}