import java.util.*;
import java.io.*;

public class Solution {
    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.`in`));
        String line = br.readLine();
        if (line == null) return;
        int n = Integer.parseInt(line.trim());
        
        StringTokenizer st = new StringTokenizer(br.readLine());
        int count = 0;
        int currMax = -1;
        for (int i = 0; i < n; i++) {
            int d = Integer.parseInt(st.nextToken());
            if (d > currMax) {
                count++;
                currMax = d;
            }
        }
        System.out.println(count);
    }
}
