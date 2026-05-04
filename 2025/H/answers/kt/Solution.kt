import java.util.*
import java.io.*

fun main() {
    val br = BufferedReader(InputStreamReader(System.`in`))
    val line = br.readLine() ?: return
    val n = line.trim().toInt()

    val st = StringTokenizer(br.readLine())
    var count = 0
    var currMax = -1
    for (i in 0 until n) {
        val d = st.nextToken().toInt()
        if (d > currMax) {
            count++
            currMax = d
        }
    }
    println(count)
}
