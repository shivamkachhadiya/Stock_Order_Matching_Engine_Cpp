Built a Low-Latency Stock Exchange Order Matching Engine in C++

Designed and implemented a high-performance price-time priority (FIFO) matching engine from scratch to simulate how modern exchanges and trading systems process millions of orders with minimal latency.

🔹 Key Highlights

• Implemented full Limit Order Book (bids/asks) using cache-friendly data structures
• O(1) order entry, cancellation, and matching operations
• Multi-threaded Producer–Consumer architecture
• Lock-free ring buffer queue for low-latency order ingestion
• Dedicated matching thread for deterministic execution
• Concurrent stress testing with 1,000,000+ simulated orders
• Achieved ~3–5 million orders/sec throughput

🔹 Tech Stack

C++17 • Multithreading • Concurrency • Lock-Free Programming • Low-Latency Systems • Data Structures • STL

🔹 What I learned

• Exchange-style matching logic
• Performance optimization & memory efficiency
• Thread synchronization and lock-free design
• Building production-style trading infrastructure

GitHub Repo:
https://github.com/shivamkachhadiya/Stock_Order_Matching_Engine_Cpp

Open to feedback and opportunities in Systems / C++ / Low-Latency / Trading Tech roles.

#cpp #systemsprogramming #lowlatency #multithreading #fintech #hft #algorithms #opensource
