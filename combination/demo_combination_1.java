/**
 * 从 100 人 中选10人得3等奖， C(100, 10) = 17310309456440
 * 再从剩下90人中选3人得2等奖，C(90, 3) = 117480
 * 再从剩下89人中选1人得1等奖, C(87, 1) = 87
 * 总共有大约有1.8 x 10 ^ 20 种可能性
 * 假设我们得计算机每1ns就能输出1个结果，全部输出来大约要5610年！
 * 假设每个结果占13个字节，把结果保存下来大约要占1995EB，远远大于世界上存储总量
 * 
 * 当数据量比较小时，还是可以算得
 */
import java.util.ArrayList;
import java.util.Arrays;

public class demo_combination_1 {

    /***
     * 求组合数
     * @param m
     * @param r
     * @return
     */

    static int c(int n, int r) {

        if (r > n) {
            return 0;
        }

        int R = n - r;
        int ret = 1;
        while (n > R) {
            ret *= n--;
        }

        while(r > 1) {
            ret /= r--;
        }
        return ret;
    }

    /**
     * 求组合情况
     * @param es
     * @param r
     * @param l 数组es开始取数位置
     * @return
     */

    static int[][] C(int[] es, int r, int l) {
        int[][] rst  = new int[c(es.length - l, r)][];
        if (r == 1) {
            for (int rsti = 0; rsti < rst.length; rsti++, l++) {
                rst[rsti] = new int[]{es[l]};
            }
        } else {
            for (int rsti = 0; l < es.length; l++) {
                int[][] srst = C(es, r - 1, l + 1);
                for (int[] sc: srst) {
                    int[] t = rst[rsti] = new int[sc.length + 1];
                    t[0] = es[l];
                    System.arraycopy(sc, 0, t, 1, sc.length);
                    rsti++;
                }
            }
        }
        return rst;
    }

    public static void main (String[] args) {
        int[][] c = C(new int[]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 3, 0);
        for (int [] cc: c) {
            System.out.println(Arrays.toString(cc));
        }
    }
}

