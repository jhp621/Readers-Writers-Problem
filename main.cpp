// Reader Preference
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t rw_mutex = PTHREAD_MUTEX_INITIALIZER;
int read_count = 0;

void* reader(void* arg) {
    pthread_mutex_lock(&mutex);
    read_count++;
    if (read_count == 1) { // 첫 번째 reader가 들어올 때 writer 락
        pthread_mutex_lock(&rw_mutex);
    }
    pthread_mutex_unlock(&mutex);

    // Reading section
    printf("Reading...\n");

    pthread_mutex_lock(&mutex);
    read_count--;
    if (read_count == 0) { // 마지막 reader가 나갈 때 writer 락 해제
        pthread_mutex_unlock(&rw_mutex);
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* writer(void* arg) {
    pthread_mutex_lock(&rw_mutex);
    // Writing section
    printf("Writing...\n");
    pthread_mutex_unlock(&rw_mutex);
    return NULL;
}


//Writer Preference
pthread_mutex_t r_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t w_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t read_try = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t resource = PTHREAD_MUTEX_INITIALIZER;
int read_count = 0, write_count = 0;

void* writer_pref_writer(void* arg) {
    pthread_mutex_lock(&w_mutex);
    write_count++;
    if (write_count == 1) pthread_mutex_lock(&read_try); // 새로운 Reader 차단
    pthread_mutex_unlock(&w_mutex);

    pthread_mutex_lock(&resource);
    // Writing section
    pthread_mutex_unlock(&resource);

    pthread_mutex_lock(&w_mutex);
    write_count--;
    if (write_count == 0) pthread_mutex_unlock(&read_try);
    pthread_mutex_unlock(&w_mutex);
    return NULL;
}

void* writer_pref_reader(void* arg) {
    pthread_mutex_lock(&read_try); // Writer가 대기 중이면 여기서 Block 됨
    pthread_mutex_lock(&r_mutex);
    read_count++;
    if (read_count == 1) pthread_mutex_lock(&resource);
    pthread_mutex_unlock(&r_mutex);
    pthread_mutex_unlock(&read_try);

    // Reading section

    pthread_mutex_lock(&r_mutex);
    read_count--;
    if (read_count == 0) pthread_mutex_unlock(&resource);
    pthread_mutex_unlock(&r_mutex);
    return NULL;
}
