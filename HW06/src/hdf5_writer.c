#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hdf5.h>

#include "hdf5_writer.h"

static int write_scalar_attribute(hid_t file_id, const char *name,
                                   hid_t h5_type, const void *value)
{
    hid_t attr_space = H5Screate(H5S_SCALAR);
    if (attr_space < 0) return -1;

    hid_t attr_id = H5Acreate2(file_id, name, h5_type, attr_space,
                                H5P_DEFAULT, H5P_DEFAULT);
    if (attr_id < 0) {
        H5Sclose(attr_space);
        return -1;
    }

    herr_t status = H5Awrite(attr_id, h5_type, value);

    H5Aclose(attr_id);
    H5Sclose(attr_space);

    return (status < 0) ? -1 : 0;
}

static int write_double_dataset(hid_t loc_id, const char *name,
                                 const double *buffer, long int length)
{
    hsize_t dims[1];
    dims[0] = (hsize_t) length;

    hid_t dataspace = H5Screate_simple(1, dims, NULL);
    if (dataspace < 0) return -1;

    hid_t dataset = H5Dcreate2(loc_id, name, H5T_NATIVE_DOUBLE, dataspace,
                                H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    if (dataset < 0) {
        H5Sclose(dataspace);
        return -1;
    }

    herr_t status = H5Dwrite(dataset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL,
                              H5P_DEFAULT, buffer);

    H5Dclose(dataset);
    H5Sclose(dataspace);

    return (status < 0) ? -1 : 0;
}

int hdf5_save_results(const char *filename,
                       const double *D, long int N,
                       int chunk_size, long int n_chunks,
                       const double *partial_chunk_sum,
                       double a)
{
    H5Eset_auto2(H5E_DEFAULT, NULL, NULL);

    hid_t file_id = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    if (file_id < 0) {
        fprintf(stderr, "hdf5_writer: impossibile creare il file '%s'\n", filename);
        return -1;
    }

    int ok = 0;

    long int n_long = N;
    ok |= write_scalar_attribute(file_id, "a", H5T_NATIVE_DOUBLE, &a);
    ok |= write_scalar_attribute(file_id, "N", H5T_NATIVE_LONG, &n_long);
    ok |= write_scalar_attribute(file_id, "chunk_size", H5T_NATIVE_INT, &chunk_size);
    ok |= write_scalar_attribute(file_id, "number_of_chunks", H5T_NATIVE_LONG, &n_chunks);

    hid_t chunks_group = H5Gcreate2(file_id, "/chunks", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    if (chunks_group < 0) {
        fprintf(stderr, "hdf5_writer: impossibile creare il gruppo /chunks\n");
        H5Fclose(file_id);
        return -1;
    }

    for (long int chunk_index = 0; chunk_index < n_chunks; chunk_index++) {

        long int current_start = chunk_index * chunk_size;
        long int current_end   = current_start + chunk_size;
        if (current_end > N) current_end = N;

        long int chunk_len = current_end - current_start;

        char dataset_name[64];
        snprintf(dataset_name, sizeof(dataset_name), "chunk_%04ld", chunk_index);

        if (write_double_dataset(chunks_group, dataset_name,
                                  D + current_start, chunk_len) != 0) {
            fprintf(stderr, "hdf5_writer: errore scrivendo il dataset '%s'\n", dataset_name);
            ok = -1;
        }
    }

    H5Gclose(chunks_group);

    if (write_double_dataset(file_id, "/partial_chunk_sum", partial_chunk_sum, n_chunks) != 0) {
        fprintf(stderr, "hdf5_writer: errore scrivendo /partial_chunk_sum\n");
        ok = -1;
    }

    if (write_double_dataset(file_id, "/D_full", D, N) != 0) {
        fprintf(stderr, "hdf5_writer: errore scrivendo /D_full\n");
        ok = -1;
    }

    H5Fclose(file_id);

    return ok;
}
