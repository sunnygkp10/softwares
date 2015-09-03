<?hh
namespace APIs\Tests\Tests\Unit\Objects;
use APIs\Infrastructure\Objects\ConcreteURL;

final class ConcreteURLTest extends \PHPUnit_Framework_TestCase {

    private $url;
    private $invalidUrl;
    public function setUp() {
        $this->url = 'http://google.com';
        $this->invalidUrl = 'this is not a url';
    }

    public function tearDown() {

    }

    public function testCreate_Success() {

        $url = new ConcreteUrl($this->url);

        $this->assertEquals($this->url, $url->get());
        $this->assertTrue($url instanceof \APIs\Domain\Https\URLs\URL);
    }

    public function testCreate_withInvalidUrl_throwsException() {

        $asserted = false;
        try {

            new ConcreteUrl($this->invalidUrl);

        } catch (\Exception $exception) {
            $asserted = true;
        }

        $this->assertTrue($asserted);

    }

}
