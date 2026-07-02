#ifndef HDF5_WRITER_H
#define HDF5_WRITER_H

int hdf5_save_results(const char *filename,
                       const double *D, long int N,
                       int chunk_size, long int n_chunks,
                       const double *partial_chunk_sum,
                       double a);

#endif /* HDF5_WRITER_H */
