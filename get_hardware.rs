
use std::fs;

fn main() {
    let cpu = fs::read_to_string("/proc/cpuinfo").unwrap();
    for line in cpu.lines() {
        if line.contains("model name") {
            println!("{}", line);
        }
    }

    let mem = fs::read_to_string("/proc/meminfo").unwrap();
    for line in mem.lines() {
        if line.contains("MemTotal") || line.contains("MemAvailable") {
            println!("{}", line);
        }
    }
}
