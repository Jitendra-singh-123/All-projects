#Name : Jitendra singh
#Section : G
#University Roll number : 2014407



########################     OBJECT DETECTION AND RECOGNITION     ################################

import numpy as np
import os
import six.moves.urllib as urllib
import sys
import tarfile
import tensorflow as tf
import zipfile
import pathlib

import cv2
from collections import defaultdict
from io import StringIO
from matplotlib import pyplot as plt
from PIL import Image
from IPython.display import display



#Import the object detection module:
from object_detection.utils import ops as utils_ops
from object_detection.utils import label_map_util
from object_detection.utils import visualization_utils as vis_util

###################################################################################
#compat.v1 allows you to write code that works both in TensorFlow 1.x and 2.x.
# patch tf1 into `utils.ops`
utils_ops.tf = tf.compat.v1

# Patch the location of gfile
tf.gfile = tf.io.gfile

###################################################################################

def load_model(model_name):
  base_url = 'http://download.tensorflow.org/models/object_detection/'
  model_file = model_name + '.tar.gz'
  model_dir = tf.keras.utils.get_file(
    fname=model_name,
    origin=base_url + model_file,
    untar=True)

  model_dir = pathlib.Path(model_dir)/"saved_model"

  model = tf.saved_model.load(str(model_dir))
  model = model.signatures['serving_default']

  return model

##########################################################################################


# List of the strings that is used to add correct label for each box.
PATH_TO_LABELS =r'C:\Users\acer\Dropbox\My PC (LAPTOP-FDJLT93A)\Downloads\tensorflow-object-detection-api-configuration-master\tensorflow-object-detection-api-configuration-master\models\research\object_detection\data\mscoco_label_map.pbtxt'
category_index = label_map_util.create_category_index_from_labelmap(PATH_TO_LABELS, use_display_name=True)

#########################################################################################

model_name = 'ssd_mobilenet_v1_coco_2017_11_17'
detection_model = load_model(model_name)

##########################################################################################

def run_inference_for_single_image(model, image):
    image = np.asarray(image)
    # The input needs to be a tensor, convert it using `tf.convert_to_tensor`.
    input_tensor = tf.convert_to_tensor(image)
    # The model expects a batch of images, so add an axis with `tf.newaxis`.
    input_tensor = input_tensor[tf.newaxis, ...]

    # Run inference
    output_dict = model(input_tensor)

    # All outputs are batches tensors.
    # Convert to numpy arrays, and take index [0] to remove the batch dimension.
    # We're only interested in the first num_detections.
    num_detections = int(output_dict.pop('num_detections'))
    output_dict = {key: value[0, :num_detections].numpy()
                   for key, value in output_dict.items()}
    output_dict['num_detections'] = num_detections

    # detection_classes should be ints.
    output_dict['detection_classes'] = output_dict['detection_classes'].astype(np.int64)

    # Handle models with masks:
    if 'detection_masks' in output_dict:
        # Reframe the the bbox mask to the image size.
        detection_masks_reframed = utils_ops.reframe_box_masks_to_image_masks(
            output_dict['detection_masks'], output_dict['detection_boxes'],
            image.shape[0], image.shape[1])
        detection_masks_reframed = tf.cast(detection_masks_reframed > 0.5,
                                           tf.uint8)
        output_dict['detection_masks_reframed'] = detection_masks_reframed.numpy()

    return output_dict

##############################################################################################


def show_inference(model, image_path):
  # the array based representation of the image will be used later in order to prepare the
  # result image with boxes and labels on it.
  image_np = image_path
  image_np=cv2.cvtColor(image_np,cv2.COLOR_BGR2RGB)
  # Actual detection.
  output_dict = run_inference_for_single_image(model, image_np)
  # Visualization of the results of a detection.
  vis_util.visualize_boxes_and_labels_on_image_array(
      image_np,
      output_dict['detection_boxes'],
      output_dict['detection_classes'],
      output_dict['detection_scores'],
      category_index,
      instance_masks=output_dict.get('detection_masks_reframed', None),
      use_normalized_coordinates=True,
      line_thickness=2)
  image_np=cv2.cvtColor(image_np,cv2.COLOR_BGR2RGB)
  return image_np

#############################################################################
def capturing(ids):
    def show_inference1(model, image_path,ids):
      # the array based representation of the image will be used later in order to prepare the
      # result image with boxes and labels on it.
      image_np = image_path
      image_np=cv2.cvtColor(image_np,cv2.COLOR_BGR2RGB)
      # Actual detection.
      output_dict = run_inference_for_single_image(model, image_np)
      # Visualization of the results of a detection.
      vis_util.visualize_boxes_and_labels_on_image_array(
          image_np,
          output_dict['detection_boxes'],
          output_dict['detection_classes'],
          output_dict['detection_scores'],
          category_index,
          instance_masks=output_dict.get('detection_masks_reframed', None),
          use_normalized_coordinates=True,
          line_thickness=2)
      image_np=cv2.cvtColor(image_np,cv2.COLOR_BGR2RGB)
      capturing_object(ids,image_np,output_dict['detection_classes'],output_dict['detection_scores'],output_dict['detection_boxes'])
      return image_np
    ################################################################################################

    from datetime import datetime


    def capturing_object(ids,image, classes, score, boxes):
        for i in range(10):
            if (classes[i] == ids and score[i] > 0.7):
                now = datetime.now()
                date_time = now.strftime("%d_%m_%Y_%H_%M_%S")
                file_name = os.path.join('D:/TEST/', date_time + '.jpg')
                cv2.imwrite(file_name, image)
                print(date_time)

    video = cv2.VideoCapture(r'D:\photos\demo.mp4')
    while (True):
        ret, img = video.read()
        img = show_inference1(detection_model, img,ids)
        ims = cv2.resize(img, (1200, 720))
        cv2.imshow('IMG', ims)
        k = cv2.waitKey(1)
        if k == 27:
            break
    cv2.destroyAllWindows()




print("Select choice from below\n")
print("1. Camera Video\n")
print("2. Custom Photo\n")
print("3. Custom Video\n")
print("4. Capturing\n")
choice = int(input())
if (choice == 2):
    while (True):
        img = cv2.imread(r'D:\photos\1_anJ8xj06Q-xr6XosDF1Etw.jpeg')
        img = show_inference(detection_model, img)
        ims = cv2.resize(img, (1200, 720))
        cv2.imshow('IMG', ims)
        k = cv2.waitKey(1)
        if (k == 27):
            break

elif choice == 1:
    video = cv2.VideoCapture(0)
    while (True):
        ret, img = video.read()
        img = show_inference(detection_model, img)
        ims = cv2.resize(img, (1200, 720))
        cv2.imshow('IMG', ims)
        k = cv2.waitKey(1)
        if (k == 27):
            break
elif choice==3:
    video = cv2.VideoCapture(r'D:\photos\demo.mp4')
    while (True):
        ret, img = video.read()
        img = show_inference(detection_model, img)
        ims = cv2.resize(img, (1200, 720))
        cv2.imshow('IMG', ims)
        k = cv2.waitKey(1)
        if (k == 27):
            break

else:
    print(category_index)
    ids=int(input("Enter the object category Index from above:"))
    capturing(ids)


cv2.destroyAllWindows()

#<------------------------------------The End--------------------------------------->#
