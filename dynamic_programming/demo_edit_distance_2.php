<?php
class EditDistance {
    
    public static function LowerOfThree($first, $second, $third) 
    {
        $min = min($first, $second);
        return min($min, $third);
    }

    public static function Levenshtein_Distance($str1, $str2)
    {

        $str1Len = strlen($str1);
        $str2Len = strlen($str2);
        $matrix = [];
        $temp = 0;
        $tempMatrix = [];

        if ($str1Len == 0 || $str2Len == 0) {
            return $str2Len;
        }

        for ($i = 0; $i <= $str1Len; $i++) {
            $matrix[$i][0] = $i;       // 初始化第一列
        }

        for ($j = 0; $j <= $str2Len; $j++) {
            $matrix[0][$j] = $j;       // 初始化第二列
        }

        /***
         * insert 理解为 x - 1 到 x 的增加的距离
         * delete 理解为 y - 1 到 的增加的距离
         * replace 理解为  x - 1, y - 1 大 x ,y 增加的距离
         */
        for ($i = 1; $i <= $str1Len; $i++) {
            $ch1 = $str1[$i - 1];
            for ($j = 1; $j <= $str2Len; $j++) {
                $ch2 = $str2[$j - 1];
                if ($ch1 == $ch2) {
                    $temp = 0;
                } else {
                    $temp = 1;
                }
                $insert = $matrix[$i - 1][$j] + 1;
                $delete = $matrix[$i][$j - 1] + 1;
                $replace = $matrix[$i - 1] [$j - 1] + $temp;
                $tempMatrix[$i][$j] = $insert . ',' . $delete . ',' . $replace;
                $matrix[$i][$j] = self::LowerOfThree($insert, $delete, $replace);
            }
        }
        // print_r($matrix);
        print_r($tempMatrix);
        echo "\r\n";
        return $matrix[$str1Len][$str2Len];
    }
}

echo "最小编辑距离为:" . EditDistance::Levenshtein_Distance('xyzab', 'axyzc');