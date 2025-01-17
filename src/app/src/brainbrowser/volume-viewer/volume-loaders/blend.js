/*
* BrainBrowser: Web-based Neurological Visualization Tools
* (https://brainbrowser.cbrain.mcgill.ca)
*
* Copyright (C) 2011
* The Royal Institution for the Advancement of Learning
* McGill University
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Affero General Public License as
* published by the Free Software Foundation, either version 3 of the
* License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Affero General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
* Author: Nicolas Kassis
* Author: Tarek Sherif <tsherif@gmail.com> (http://tareksherif.ca/)
*/

/**
 * Represents a blended volume. It basically just puts two volumes on top of each other.
 * Registering of the volumes is needed though, otherwise blending does not make sense.
 */

(function() {
  "use strict";

  var VolumeViewer = BrainBrowser.VolumeViewer;
  var image_creation_context = document.createElement("canvas").getContext("2d");

  VolumeViewer.volume_loaders.blend = function(options, callback) {
    options = options || {};
	
    var volumes = options.volumes || [];
    var header = volumes[0].header;
    var overlay_volume = VolumeViewer.createVolume(header, []);

    overlay_volume.type = "blend";
    overlay_volume.volumes = [];
    overlay_volume.blend_ratios = [];

    overlay_volume.slice = function(axis, slice_num, time) {
      slice_num = slice_num === undefined ? this.position[axis] : slice_num;
      time = time === undefined ? this.current_time : time;

      var slices = [];

      this.volumes.forEach(function(volume) {
        var slice = volume.slice(axis, slice_num, time);
        slices.push(slice);
      });

      return {
        height_space: header[axis].height_space,
        width_space: header[axis].width_space,
        slices: slices
      };
    };
	
	//Need to improve this. Pretty slow
    overlay_volume.getSliceImage = function(slice, zoom, contrast, brightness) {
      zoom = zoom || 1;

      var slices = slice.slices;
      var images = [];

      slices.forEach(function(slice, i) {
        var volume = overlay_volume.volumes[i];
        images.push(volume.getSliceImage(slice, zoom, contrast, brightness));
      });

      return blendImages(
        images,
        overlay_volume.blend_ratios,
        image_creation_context.createImageData(images[0].width,images[0].height)
      );
    };

    volumes.forEach(function(volume) {
      overlay_volume.volumes.push(volume);
      overlay_volume.blend_ratios.push(1 / volumes.length);
    });

    if (BrainBrowser.utils.isFunction(callback)) {
      callback(overlay_volume);
    }
  };

  // Blend the pixels of several images using the alpha value of each
  function blendImages(images, blend_ratios, target) {
    var num_images = images.length;

    if (num_images === 1) {
      return images[0];
    }

    var target_data = target.data;
    var width = target.width;
    var height = target.height;
    var y, x;
    var i;
    var image, image_data, pixel, alpha, current;
    var row_offset;

    //This will be used to keep the position in each image of its next pixel
    var image_iter = new Uint32Array(images.length);
    var alphas = new Float32Array(blend_ratios);

    for (y = 0; y < height; y += 1) {
      row_offset = y * width;

      for (x = 0; x < width; x += 1) {
        pixel = (row_offset + x) * 4;
        alpha = 0;

        for (i = 0; i < num_images; i += 1) {
          image = images[i];

          if(y < image.height &&  x < image.width) {

            image_data = image.data;

            current = image_iter[i];

            //Red
            target_data[pixel] = target_data[pixel] * alpha +
                                  image_data[current] * alphas[i];

            //Green
            target_data[pixel + 1] = target_data[pixel + 1] * alpha +
                                      image_data[current + 1] * alphas[i];

            //Blue
            target_data[pixel + 2] = target_data[pixel + 2] * alpha +
                                      image_data[current + 2] * alphas[i];

            target_data[pixel + 3] = 255;
            alpha += alphas[i];

            image_iter[i] += 4;
          }

        }

      }
    }

    return target;

  }

}());

