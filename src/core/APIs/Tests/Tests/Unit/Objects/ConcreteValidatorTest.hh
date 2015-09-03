<?hh
namespace APIs\Tests\Tests\Unit\Objects;
use APIs\Domain\Configurations\Validators\Validator;
use APIs\Infrastructure\Objects\ConcreteValidator;

final class ConcreteValidatorTest extends \PHPUnit_Framework_TestCase {
    private $functionCodeMock;
    private $validatorMock;
    private $subValidators;
    private $emptySubValidators;
    public function setUp() {

        $this->functionCodeMock = $this->getMock('APIs\Domain\Configurations\Functions\FunctionCode');
        $this->validatorMock = $this->getMock('APIs\Domain\Configurations\Validators\Validator');

        $this->subValidators = Map<string, Validator> {
            'one_sub_validator' => $this->validatorMock
        };

        $this->emptySubValidators = Map<string, Validator> {};

    }

    public function tearDown() {

    }

    public function testCreate_Success() {

        $validator = new ConcreteValidator($this->functionCodeMock);

        $this->assertEquals($this->functionCodeMock, $validator->getFunctionCode());
        $this->assertFalse($validator->hasSubValidators());
        $this->assertNull($validator->getSubValidators());

        $this->assertTrue($validator instanceof \APIs\Domain\Configurations\Validators\Validator);

    }

    public function testCreate_withEmptySubValidators_Success() {

        $validator = new ConcreteValidator($this->functionCodeMock, $this->emptySubValidators);

        $this->assertEquals($this->functionCodeMock, $validator->getFunctionCode());
        $this->assertFalse($validator->hasSubValidators());
        $this->assertNull($validator->getSubValidators());

    }

    public function testCreate_withSubValidators_Success() {

        $validator = new ConcreteValidator($this->functionCodeMock, $this->subValidators);

        $this->assertEquals($this->functionCodeMock, $validator->getFunctionCode());
        $this->assertTrue($validator->hasSubValidators());
        $this->assertEquals($this->subValidators, $validator->getSubValidators());

    }

}
