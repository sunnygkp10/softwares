<?hh
namespace APIs\Infrastructure\Adapters;
use APIs\Domain\Https\URLs\URLAdapter;
use APIs\Infrastructure\Objects\ConcreteURL;
use APIs\Domain\Https\URLs\URL;

final class ConcreteURLAdapter implements URLAdapter {

    public function __construct() {

    }

    public function convertStringToURL(string $url): URL {

        try {

            return new ConcreteURL($url);

        } catch (\Exception $exception) {
            throw $exception;
        }

    }

}
