import socket
import time
import threading

HOST = "127.0.0.1"
PORT = 8080
REQUESTS_PER_THREAD = 1000
NUM_THREADS = 10

total_requests = NUM_THREADS * REQUESTS_PER_THREAD
successful_ops = 0
lock = threading.Lock()

def worker(thread_id):
    global successful_ops
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((HOST, PORT))
        local_success = 0
        
        for i in range(REQUESTS_PER_THREAD):
            key = f"key_{thread_id}_{i}"
            val = f"val_{i}"
            # Write
            s.sendall(f"SET {key} {val}\n".encode())
            res = s.recv(1024)
            if b"OK" in res:
                local_success += 1
            # Read
            s.sendall(f"GET {key}\n".encode())
            res = s.recv(1024)
            if val.encode() in res:
                local_success += 1
                
        s.close()
        with lock:
            successful_ops += local_success
    except Exception as e:
        print(f"Thread {thread_id} error: {e}")

print(f"Starting benchmark: {NUM_THREADS} concurrent threads, total operations: {total_requests * 2}...")
start_time = time.time()

threads = []
for t in range(NUM_THREADS):
    th = threading.Thread(target=worker, args=(t,))
    threads.append(th)
    th.start()

for th in threads:
    th.join()

elapsed = time.time() - start_time
throughput = successful_ops / elapsed

print("\n--- BENCHMARK RESULTS ---")
print(f"Total Completed Operations: {successful_ops}")
print(f"Total Time Taken:          {elapsed:.2f} seconds")
print(f"Throughput (Ops/Sec):       {throughput:.2f} req/sec")
print(f"Average Latency:           {(elapsed / successful_ops) * 1000:.3f} ms/req")
