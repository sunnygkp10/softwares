<?hh
namespace APIs\Tests\Tests\Unit\Objects;
use APIs\Infrastructure\Objects\ConcreteInputValidator;

final class ConcreteInputValidatorTest extends \PHPUnit_Framework_TestCase {
    private $variableMock;
    private $validatorMock;
    public function setUp() {

        $this->variableMock = $this->getMock('APIs\Domain\Configurations\Variables\Variable');
        $this->validatorMock = $this->getMock('APIs\Domain\Configurations\Validators\Validator');

    }

    public function tearDown() {

    }

    public function testCreate_Success() {

        $inputValidator = new ConcreteInputValidator($this->variableMock, $this->validatorMock);

        $this->assertEquals($this->variableMock, $inputValidator->getVariable());
        $this->assertEquals($this->validatorMock, $inputValidator->getValidator());

        $this->assertTrue($inputValidator instanceof \APIs\Domain\Configurations\Validators\Input\InputValidator);

    }
}
