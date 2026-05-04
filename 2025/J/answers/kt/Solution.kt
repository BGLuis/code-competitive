import java.util.*

fun main() {
    val sc = Scanner(System.`in`)
    if (!sc.hasNext()) return
    val s = sc.next()

    var score = 0L
    val n = s.length
    for (i in 0 until n) {
        if (s.startsWith("ha", i)) score += 1
        if (s.startsWith("boooo", i)) score -= 1
        if (s.startsWith("bravo", i)) score += 3
    }
    println(score)
}
