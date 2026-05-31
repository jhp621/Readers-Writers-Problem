Evaluation
이 문제는 읽기 작업과 쓰기 작업 간의 우선순위를 어떻게 두느냐에 따라 시스템의 동작이 크게 달라집니다. 
두 가지 방식을 구현하고 동작을 비교했습니다.  
Reader Preference
첫 번째 Reader가 임계 구역에 진입할 때 pthread_mutex_lock(&rw_mutex)로 Writer의 접근을 차단하고
이후 진입하는 Reader들은 read_count만 증가시키며 자유롭게 데이터를 읽습니다. 
마지막 Reader가 빠져나갈 때 비로소 rw_mutex를 해제합니다. 
이 방식은 읽기 작업의 병행성(Concurrency)은 극대화되지만 Reader가 끊임없이 들어온다면 
Writer는 영원히 락을 획득하지 못하는 Writer Starvation이 발생할 수 있습니다.
Writer Preference 
Writer Starvation을 해결하기 위해 read_try라는 추가적인 Mutex를 도입했습니다.
