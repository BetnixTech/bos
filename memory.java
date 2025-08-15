public class Example {
    public static void main(String[] args) {
        int cores = Runtime.getRuntime().availableProcessors();
        long freeMem = Runtime.getRuntime().freeMemory() / (1024*1024);
        long totalMem = Runtime.getRuntime().totalMemory() / (1024*1024);
        String os = System.getProperty("os.name") + " " + System.getProperty("os.arch");

        System.out.println("Operating System: " + os);
        System.out.println("CPU Cores: " + cores);
        System.out.println("Total Memory (MB): " + totalMem);
        System.out.println("Free Memory (MB): " + freeMem);
    }
}
