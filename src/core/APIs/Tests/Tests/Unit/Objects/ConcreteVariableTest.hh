<?hh
namespace APIs\Tests\Tests\Unit\Objects;
use APIs\Infrastructure\Objects\ConcreteVariable;

final class ConcreteVariableTest extends \PHPUnit_Framework_TestCase {
    private $delimiter;
    private $variable;
    private $keynames;
    private $invalidVariable;
    private $consecutiveDelimitersInVariable;
    private $delimiteAtTheEndVariable;
    public function setUp() {

        $this->delimiter = '->';
        $this->variable = 'my'.$this->delimiter.'var';
        $this->keynames = Vector<string> {'my', 'var'};
        $this->invalidVariable = 'the delimiter is not present in this variable.';
        $this->consecutiveDelimitersInVariable = 'my'.$this->delimiter.$this->delimiter.'var';
        $this->delimiteAtTheEndVariable = 'my'.$this->delimiter.'var'.$this->delimiter;

    }

    public function tearDown() {

    }

    public function testCreate_Success() {

        $inputValidator = new ConcreteVariable($this->variable, $this->delimiter);

        $this->assertEquals($this->variable, $inputValidator->getName());
        $this->assertEquals($this->keynames, $inputValidator->getKeynames());

        $this->assertTrue($inputValidator instanceof \APIs\Domain\Configurations\Variables\Variable);

    }

    public function testCreate_withEmptyVariable_throwsException() {

        $asserted = false;
        try {

            new ConcreteVariable('', $this->delimiter);

        } catch (\Exception $exception) {
            $asserted = true;
        }

        $this->assertTrue($asserted);

    }

    public function testCreate_withEmptyDelimiter_throwsException() {

        $asserted = false;
        try {

            new ConcreteVariable($this->variable, '');

        } catch (\Exception $exception) {
            $asserted = true;
        }

        $this->assertTrue($asserted);

    }

    public function testCreate_withDelimiterNotFoundInVariable_throwsException() {

        $asserted = false;
        try {

            new ConcreteVariable($this->invalidVariable, $this->delimiter);

        } catch (\Exception $exception) {
            $asserted = true;
        }

        $this->assertTrue($asserted);

    }

    public function testCreate_withConsecutiveDelimitersInVariable_throwsException() {

        $asserted = false;
        try {

            new ConcreteVariable($this->consecutiveDelimitersInVariable, $this->delimiter);

        } catch (\Exception $exception) {
            $asserted = true;
        }

        $this->assertTrue($asserted);

    }

    public function testCreate_withDelimiterAtTheEndVariable_throwsException() {

        $asserted = false;
        try {

            new ConcreteVariable($this->delimiteAtTheEndVariable, $this->delimiter);

        } catch (\Exception $exception) {
            $asserted = true;
        }

        $this->assertTrue($asserted);

    }

}
