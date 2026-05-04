import java.util.*

fun factorize(n: Long, factors: MutableMap<Long, Long>) {
    var temp = n
    var i = 2L
    while (i * i <= temp) {
        while (temp % i == 0L) {
            factors[i] = (factors[i] ?: 0L) + 1L
            temp /= i
        }
        i++
    }
    if (temp > 1) factors[temp] = (factors[temp] ?: 0L) + 1L
}

fun main() {
    val sc = Scanner(System.`in`)
    if (!sc.hasNext()) return
    val s = sc.next()
    val n = s.length

    val factors = TreeMap<Long, Long>()
    if (n <= 15) {
        val value = s.toLong()
        factorize(value, factors)
    } else {
        // Take first 14 digits for efficient trial division
        val p = s.substring(0, 14).toLong()
        val l = (n - 14).toLong()
        factorize(p, factors)
        factors[2L] = (factors[2L] ?: 0L) + l
        factors[5L] = (factors[5L] ?: 0L) + l
    }

    val res = factors.filter { it.value > 0 }
    println(res.size)
    for ((p, e) in res) {
        println("$p $e")
    }
}
