<?hh
namespace APIs\Tests\Tests\Unit\Objects;
use APIs\Infrastructure\Objects\ConcreteRenderedParser;

final class ConcreteRenderedParserTest extends \PHPUnit_Framework_TestCase {
    private $functionCodeMock;
    public function setUp() {

        $this->functionCodeMock = $this->getMock('APIs\Domain\Configurations\Functions\FunctionCode');

    }

    public function tearDown() {

    }

    public function testCreate_Success() {

        $renderedParser = new ConcreteRenderedParser($this->functionCodeMock);

        $this->assertEquals($this->functionCodeMock, $renderedParser->getFunctionCode());

        $this->assertTrue($renderedParser instanceof \APIs\Domain\Configurations\Rendered\RenderedParsers\RenderedParser);

    }

}
