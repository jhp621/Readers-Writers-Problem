Evaluation:
이 문제는 읽기 작업과 쓰기 작업 간의 우선순위를 어떻게 두느냐에 따라 시스템의 동작이 크게 달라집니다. 
두 가지 방식을 구현하고 동작을 비교했습니다.  

Reader Preference:
첫 번째 Reader가 임계 구역에 진입할 때 pthread_mutex_lock(&rw_mutex)로 Writer의 접근을 차단하고
이후 진입하는 Reader들은 read_count만 증가시키며 자유롭게 데이터를 읽습니다. 
마지막 Reader가 빠져나갈 때 비로소 rw_mutex를 해제합니다. 
이 방식은 읽기 작업의 병행성(Concurrency)은 극대화되지만 Reader가 끊임없이 들어온다면 
Writer는 영원히 락을 획득하지 못하는 Writer Starvation이 발생할 수 있습니다.

Writer Preference:
Writer Starvation을 해결하기 위해 read_try라는 추가적인 Mutex를 도입했습니다.
대기 중이거나 작업 중인 Writer가 한 명이라도 생기면 즉시 read_try 락을 걸어버립니다. 
이로 인해 새롭게 진입하려는 Reader는 read_try에서 블로킹되어 더 이상 진입하지 못하게 되며
기존에 있던 Reader들이 작업을 마치면 Writer가 즉시 작업을 수행할 수 있도록 보장합니다.

결론 및 느낀점:
두 코드를 비교 구현하면서 완벽한 동기화 기법이란 존재하지 않으며 
시스템의 목적과 워크로드 특성에 따라 적절한 정책을 선택해야 함을 깨달았습니다. 
데이터베이스의 조회수와 같이 읽기가 압도적으로 많은 시스템에서는 Reader Preference가 유리하지만
은행 계좌 잔액 갱신처럼 데이터의 일관성과 최신화가 중요한 시스템에서는 Writer Preference를 채택하여 
갱신 지연을 막아야 한다는 운영체제 설계의 트레이드오프를 깊이 이해할 수 있었습니다.
