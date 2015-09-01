<?hh
function nonZeroInteger(string $input) {
    $matches = array();
    preg_match_all('/[0-9]+/s', $input, $matches);

    if (isset($matches[0][0]) && ($matches[0][0] == $input)) {
        $value = (int) $matches[0][0];
        if ($value == 0) {
            throw new \Exception('The given input cannot be 0.', 500);
        }

        return $value;
    }

    throw new \Exception('The given input ('.$input.') is not a valid non-zero integer.', 500);
};
