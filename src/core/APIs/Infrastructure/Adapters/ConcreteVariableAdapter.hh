<?hh
namespace APIs\Infrastructure\Adapters;
use APIs\Domain\Configurations\Variables\VariableAdapter;
use APIs\Infrastructure\Objects\ConcreteVariable;
use APIs\Domain\Configurations\Variables\Variable;

final class ConcreteVariableAdapter implements VariableAdapter {
    private $prefix;
    private $suffix;
    private $pointer;
    public function __construct(string $prefix, string $suffix, string $pointer) {
        $this->prefix = $prefix;
        $this->suffix = $suffix;
        $this->pointer = $pointer;
    }

    public function convertStringToVariable(string $variable): Variable {

        $beginPos = strpos($variable, $this->prefix);
        $endPos = strrpos($variable, $this->suffix);

        if (($beginPos === false) || ($endPos === false)) {
            throw new \Exception('The variable ('.$variable.') is invalid.  The variable prefix/suffix ('.$this->prefix.'/'.$this->suffix.') cannot be found in it.');
        }

        $beginPos += 1;
        $length = $endPos - $beginPos;
        $variable = substr($variable, $beginPos, $length);
        return new ConcreteVariable($variable, $this->pointer);
    }

}
