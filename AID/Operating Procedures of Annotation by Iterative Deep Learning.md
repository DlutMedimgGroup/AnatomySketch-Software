# Operating Procedures of Annotation by Iterative Deep Learning

Since 3D medical data labeling takes a lot of time, the problem of lack of labels is often encountered when training deep neural networks. This hinders the use of the excellent method of deep network to solve practical problems. This article introduces a method of iterative annotation and training using AnatomySketch (AS). The main process of this method includes (a) Annotate a small amount of data (for example, 5 cases) to train an initial network. (b) Use the initial network to segment more data to get their labels, although these labels may not be perfectly accurate. （c) Use AnatomySketch to correct these tags. Correction is easier than annotating from scratch, which reduces the workload. (d) Use a larger training set to train a better performance network. This process can continue until enough labels are obtained and a network with sufficient performance is trained. Sample data and models can be downloaded [here](https://1drv.ms/u/s!AiAogjEIFaXOgcM-sEa-0yiM1wZsfQ?e=Xema6Z).

Tips: The minimum version of AnatomySketch is 1.1.

## Initial annotation

You can determine the amount of data used for the first training based on the size of the data set. Since the network used is 2D, it is enough to annotate about 5 cases of data at the beginning. The following describes the method of using AnatomySketch to annotate data. After opening the AS software, use the "Open" or “DICOM” button to import the data. Let’s take an example of annotating a left kidney in CT data. The name of data used in the example is "102_image". 

<img src=".\figures\fig1.png" style="zoom:80%;" />

The basic functions of the software can be viewed by clicking the "Help" button in the menu bar. Right-click on "112_image" in the upper left data list and select "New Segmentation". After creating a new segmentation, select the segmentation algorithm "Contour_Interpolation" in the drop-down menu at the bottom left.

<img src=".\figures\fig2.png" style="zoom:80%;" />

Select "New Contour" in the "Target" drop-down list, and then the new data item will be displayed in the upper left data list. At this time, the software will enter the state of contour painting. Hold down the ctrl key throughout the process, and click the left mouse button to place the control points on the edge of the kidney to complete the contour line drawing. 

<img src=".\figures\fig3.png" style="zoom:80%;" />

After selecting a direction (such as axial), all contour lines are painted in that direction. It is not necessary to paint every layer, just paint 5-7 layers evenly across the kidney. The number of layers depends on the size and complexity of the target. A small suggestion is to include the top and bottom of the liver in the painted layers. After finishing the painting, set “Smooth” to 50 and click "Calculate". 

<img src=".\figures\fig4.png" style="zoom:80%;" />

Anatomy Sketch also supports fine adjustment of the results of the segmentation. Click Segmentation_0_result in the upper left data list to enter the mesh adjustment mode. Then select the only mesh to be adjusted in the lower left page. 

<img src=".\figures\fig5.png" style="zoom:80%;" />

At this time, in the three 2D views, you can drag the mesh while holding down "Ctrl". This correction is 3D, all views will change simultaneously. The software will determine the scope of influence of each operation according to the scope of the view. If you need to adjust the range of influence, you can hold down the Ctrl key and scroll the mouse wheel after you finish dragging. After you are satisfied with the result, you can right-click the poly data (Segmentation_0_result) in the upper left corner to save. In order to cooperate with the subsequent data processing, it is recommended to save the results in the *.stl format.

## Initial Network Training

The following introduction and sample code take as an example the multi-organ segmentation containing four organs: kidney, left kidney, right kidney, and spleen. Place the original CT image under the “./image” path, and place the marked mesh under the “./mesh” path. Change the name of the original CT to 101_imgae, 102_image... Change the name of the corresponding STL file to 101_liver_mesh, 101_leftkidney_mesh, 101_rightkidney_mesh, 101_spleen_mesh, 102_liver_mesh... Then run "./python/dataset_generate.py", and the result will be saved in "./slice_dataset". (This corresponds to the "0_Slice_Data" folder in [the sample data](https://1drv.ms/u/s!AiAogjEIFaXOgcM-sEa-0yiM1wZsfQ?e=Xema6Z).) In the folder there are three folders: “annotations”, “img”, and “mask”, which are the annotations of the slice data, the slice data itself, and the slice label that is convenient for users to observe. For the process of generating 2D slice data, there are several points to note. The format of the generated 2D data is *.jpg. The window level used in the conversion of gray value is [400, 20]. In addition, different slices of the kidney and spleen are set to the same label, while the liver is set to three different labels from top to bottom.

After the grouped slice data is obtained, the initial network training can be carried out. At this time, we need a workstation with Linux operating system and complete the network environment configuration. (Copy the "snake" folder to Linux system or just clone from this [site](https://github.com/zju3dv/snake).) Copy the two folders "annotations" and "img" of the previously generated Group_1 data to the "./data/MyCT/" path of the segmented network project in the Linux workstation. Use this statement to train the network in the project root directory: “python train_net.py --cfg_file configs/my_ct_snake.yaml model my_ct_snake”. 

We used 4 NVIDIA Tesla P40 GPUs during network training and 300 epochs iterated. The initial lr is 1e-4, and it is halved at (80, 120, 150, 170) epochs. If you need to modify the network parameters, you can edit "./configs/my_ct_snkae.yaml". The trained network model file will be stored in "./data/model/snake/my_ct_snake".

The trained model corresponds to the "1_Initial_Pth" folder in [the sample data](https://1drv.ms/u/s!AiAogjEIFaXOgcM-sEa-0yiM1wZsfQ?e=Xema6Z).

## Initial Network Training Inference

After completing the training, use the trained network to segment more data. Use the same method as the previous training data to generate a sliced data set of the augmented data. The difference from the previous steps is that there is no file in the "./mesh" path at this time. Also under "./slice_dataset" there will be three folders "annotations", "img", and "mask". Copy "annotations" and "img" to the "./data/MyCTVal" path of the Linux system segmentation network project. Run the following command in the project root director:

 “python run.py --type visualize --cfg_file configs/my_ct_snake.yaml test.dataset MyCTVal ct_score 0.3 model my_ct_snake”. The network output results are stored in "./data/result". Copy the output results of the network twice back to the local Windows computer and save it in: "./network_ouput".

The direct output format of the network is json, and you need to run “./python/post_processing.py” to convert it into a 3D label map and contour file (This corresponds to the "2_Results_3d_Label" and "3_Results_3d_ctr" in [the sample data](https://1drv.ms/u/s!AiAogjEIFaXOgcM-sEa-0yiM1wZsfQ?e=Xema6Z).). In addition to the file format conversion, the post-processing program also performs morphological operations, including opening operations, closing operations, and extracting the largest connected domain. 

## Interactive Correction

The results of the initial network are not completely accurate, so manual correction is required. Anatomy Sketch (AS) provides simple and intuitive interactive correction tools. After opening the AS software, use the "Open" button to import the CT data and contours that need to be corrected (e.g. 112_image.nii.gz & 112.ctr). The interface of the software after importing the data is shown in Figure.

<img src=".\figures\fig6.png" style="zoom:80%;" />

Right-click on "112_image" in the upper left data list and select "New Segmentation". After creating a new segmentation, select the segmentation algorithm "Contour_InterpolationPlus" in the drop-down menu at the bottom left. Right-click on "Contour Data" in the upper left data list and select "Move to". Select the newly created partition, such as "Segmentation_0", as the moving target. The data list at this time should be as shown in Figure.

<img src=".\figures\fig7.png" style="zoom:80%;text-align: center;" />

In the algorithm parameter interface at the bottom left of the software, set “Target” as the target organ, such as “leftkidney”. Direction represents the direction of interpolation, and the default 2 represents axial. In addition, 0 means sagittal; 1 means coronal. 

<img src=".\figures\fig8.png" style="zoom:80%;text-align: center;" />

In the next step, you can start to correct the contour. Double-click the corresponding contour "leftkidney" at the upper left to make it bold. Select an appropriate slice in the view to make corrections. While holding down the space, you can use the mouse to drag the outline to complete the correction. In addition, you can also press and hold "d" and click a control point to delete it; or press and hold "a" and click the mouse to add a control point. After finishing the correction, hold down “s” and click any control point, and the outline will be thickened at this time. This means that the contour line is selected, and only the selected contour line will be input into the algorithm. For a relatively small organ like the liver, it is more appropriate to correct 5-7 layers. Try to make these layers evenly distributed and include the top and bottom layers. Finally click the "Calculate" button to get the result. 

<img src=".\figures\fig9.png" style="zoom:80%;" />

The result of the correction can be further corrected. At this time, there should be a "Segmentation_0_result" in the data list in the upper left corner, which represents the surface output by the algorithm. Click it, and then select the only contour, as shown in Figure 10. At this time, in the three 2D views, you can drag the poly while holding down "Ctrl". After you are satisfied with the result, you can right-click the poly data (Segmentation_0_result) in the upper left corner to save.

<img src=".\figures\fig10.png" style="zoom:80%;" />

The correction result corresponds to the "4_Results_Corrected" folder in [the sample data](https://1drv.ms/u/s!AiAogjEIFaXOgcM-sEa-0yiM1wZsfQ?e=Xema6Z).

## Follow-up Network Training

At this time, we have obtained more accurate data labels through corrections. At this point we can use more data to train a better performance network with the same steps as the initial training. If the performance is still not good enough, you can continue this iterative process. [The sample data](https://1drv.ms/u/s!AiAogjEIFaXOgcM-sEa-0yiM1wZsfQ?e=Xema6Z) also shows a improve model in the "5_Improved_Pth" folder.