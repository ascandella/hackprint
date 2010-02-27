/*
 * Adapted from the verify.c example program, written by Daniel Drake
 * <dsd@gentoo.org>, 2007.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <libfprint/fprint.h>

#define PASSWORD "haxorz"
#define AUTHPIPE "/opt/hackprint/authpipe"

// These functions lifted from pam_fprint.c (part of the pam_fprint library)
// released under the GNU General Public License v2
 static struct fp_print_data **find_dev_and_prints(struct fp_dscv_dev **ddevs,
 	struct fp_dscv_print **prints, struct fp_dscv_dev **_ddev, enum fp_finger **fingers)
 {
 	int i = 0, j = 0, err;
 	struct fp_dscv_print *print;
 	struct fp_dscv_dev *ddev = NULL;
 	uint16_t driver_id, driver_id_cur;
 	size_t prints_count = 0;
 	struct fp_print_data **gallery;

 	/* TODO: add device selection */
 	while (print = prints[i++]) {
 		if (!ddev) {
 			ddev = fp_dscv_dev_for_dscv_print(ddevs, print);
 			driver_id = fp_dscv_print_get_driver_id(print);
 			*_ddev = ddev;
 		}
 		if (ddev)
 		{
 		    driver_id_cur = fp_dscv_print_get_driver_id(print);
 		    if (driver_id_cur == driver_id) {
 			    prints_count++;
 		    }
 		}
 	}

 	if (prints_count == 0) {
 	    return NULL;
 	}

 	gallery = malloc(sizeof(*gallery) * (prints_count + 1));
 	if (gallery == NULL) {
 	    return NULL;
 	}
 	gallery[prints_count] = NULL;
 	*fingers = malloc(sizeof(*fingers) * (prints_count));
 	if (*fingers == NULL) {
 	    free(gallery);
 	    return NULL;
 	}

 	i = 0, j = 0;
 	while (print = prints[i++]) {
 		driver_id_cur = fp_dscv_print_get_driver_id(print);
 		if (driver_id_cur == driver_id) {
 			err = fp_print_data_from_dscv_print(print, & (gallery[j]));
 			if (err != 0) {
 			    gallery[j] = NULL;
 			    break;
 			}
 			(*fingers)[j] = fp_dscv_print_get_finger(print);
 			j++;
 		}
 	}

 	return gallery;
 }


int verify(struct fp_dev *dev, struct fp_print_data **gallery)
{
    int r;
    size_t match_offset;

    do {
        sleep(1);
        printf("\nScan your finger now.\n");
        r = fp_identify_finger(dev, gallery, &match_offset);
        
        if (r < 0) {
            printf("verification failed with error %d :(\n", r);
            return r;
        }
        handle_output(r);
    } while (1);
}

void handle_output(int result)
{
    FILE* authpipe;
    switch (r) {
        case FP_VERIFY_NO_MATCH:
            printf("NO MATCH\n");
            return 0;
        case FP_VERIFY_MATCH:
            // Write the magic password to the pipe
            authpipe = fopen(AUTHPIPE, 'w');
            fprintf(authpipe, PASSWORD); 
            fclose(authpipe);
        
            printf("Match\n");
            return 0;
        case FP_VERIFY_RETRY:
            printf("Scan didn't quite work. Please try again.\n");
            break;
        case FP_VERIFY_RETRY_TOO_SHORT:
            printf("Swipe was too short, please try again.\n");
            break;
        case FP_VERIFY_RETRY_CENTER_FINGER:
            printf("Please center your finger on the sensor and try again.\n");
            break;
        case FP_VERIFY_RETRY_REMOVE_FINGER:
            printf("Please remove finger from the sensor and try again.\n");
            break;
    }
}

static int do_auth()
{
    int r;
	struct fp_dscv_dev **ddevs;
	struct fp_dscv_print **prints;
	struct fp_dscv_dev *ddev;
	struct fp_dscv_print *print;
	struct fp_dev *dev;
	struct fp_print_data **gallery, **gallery_iter;
	enum fp_finger *fingers;
 
	r = fp_init();
	if (r < 0) {
        fprintf(stderr, "Could not initialize libfprint.");
        return -1;
	}
 
	ddevs = fp_discover_devs();
	if (!ddevs) {
        fprintf(stderr, "Could not discover any devices.");
        return 1;
	}
 
	prints = fp_discover_prints();
	if (!prints) {
		fp_dscv_devs_free(ddevs);
        fprintf(stderr, "Could not discover any fingerprints.");
        return 2;
	}
 
	gallery = find_dev_and_prints(ddevs, prints, &ddev, &fingers);
	if (!gallery) {
		fp_dscv_prints_free(prints);
		fp_dscv_devs_free(ddevs);
		fprintf(stderr, "Could not locate any suitable fingerprints "
			"matched with available hardware.");
        return 3;
	}
 
	dev = fp_dev_open(ddev);
	fp_dscv_devs_free(ddevs);
	fp_dscv_prints_free(prints);
	if (!dev) {
		gallery_iter = gallery;
		while (*gallery_iter) {
		    fp_print_data_free(*gallery_iter);
		    gallery_iter++;
		}
		free(gallery);
		free(fingers);	
        fprintf(stderr, "No fingerprint information available.");
        return 4;
	}
	
	r = verify(dev, gallery);
 
    // Free up fingreprint information
	gallery_iter = gallery;
	while (*gallery_iter)
	{
	    fp_print_data_free(*gallery_iter);
	    gallery_iter++;
	}
	free(gallery);
	free(fingers);
	fp_dev_close(dev);
    return 0;
}

int main(void)
{
    do_auth();
}


