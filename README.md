# AnatomySketch-Software

AnatomySkectch (AS)  is a scientific research software developed by the [**Medical Imaging Research Group of Dalian University of Technology**](http://biomedimg-dlut-edu.cn/index.html) for segmentation, annotation and automated analysis of medical images. Its main features include:

- Interactivity: Support mouse, touch screen, and stylus operation to facilitate segmentation and annotation of medical images. The software interface design strives to be concise and provide convenience for interaction.
- Scalability: Support users to package self-compiled C++ or Python programs into plug-ins and integrate them into software (such as deep neural networks, etc.), which facilitates the realization of personalized functions in the graphical interface, and makes up for the lack of interactive interface carriers for current deep learning algorithms Defects. The software also comes with basic image and mesh processing functions (such as cropping, filtering, basic segmentation, mesh processing, DICOM image loading) to meet basic image processing functions.
- Language: The software supports English or Chinese interface, which is convenient for various users.
- Open source: software, including source code, is open source and free.

Everyone is welcome to download and try this software. After you download the software, open the software and click "Help" to read the detailed introduction of the software. User manual is provided [here](https://github.com/DlutMedimgGroup/AnatomySketch-Software/blob/master/UserManual.md) in English and [here](https://github.com/DlutMedimgGroup/AnatomySketch-Software/blob/master/UserManual_ch.md) in Chinese.

This is not a software for clinical applications. Our purpose is to provide a convenient prototype algorithm development platform for domestic engineers and doctors engaged in scientific research. We strive to establish a medical image processing algorithm exchange community based on this in the near future, and contribute to the first step in the transformation of scientific research results into clinical practice.

Because the software is completely developed by the graduate students in school. Just like the first-generation iPhone, there are inevitably various defects. Please feel free to criticize. The development team sincerely invites experts with software development experience to join (paid) to jointly promote the progress of such an open source platform and achieve a win-win situation.

Development team:
Zhuang Mingrui (Ph.D student), Yang Yuxin (Undergraduate student), Liu Hao (Master student), Guo Yuntao (Undergraduate student), Lin Bolin (Undergraduate student), Wang Hongkai (Supervisor)

## Download 

To download software, you need to click [release](https://github.com/DlutMedimgGroup/AnatomySketch-Software/releases) in the Github project, and select the version. If you only want to run the software, download AnatomySketch-x.x.zip, no need to download the remaining files. The dense_vnet_abdominal_ct.zip and vgg19_se_block_model.zip in the remaining files are examples of deep learning plug-ins, which are described in more detail in the software manual. Source code is the software source code, used for software compilation. 

##  Annotation by Iterative Deep Learning (AID)

Since 3D medical data labeling takes a lot of time, the problem of lack of labels is often encountered when training deep neural networks. This hinders the use of the excellent method of deep network to solve practical problems. [This article](https://github.com/DlutMedimgGroup/AnatomySketch-Software/blob/master/AID/Operating%20Procedures%20of%20Annotation%20by%20Iterative%20Deep%20Learning.md) introduces a method of iterative annotation and training using AnatomySketch.

## Compile

If you want to compile the source code of this software, you need to install cmake. Use cmake to compile the sln project, and then use Visual Studio to compile the project. The project is written in c/c++ and can only be run in the windows environment. The libraries needed to compile the project are: Qt, VTK, ITK.
