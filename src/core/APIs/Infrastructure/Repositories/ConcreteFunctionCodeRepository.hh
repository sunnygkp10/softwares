<?hh
namespace APIs\Infrastructure\Repositories;
use APIs\Domain\Configurations\Functions\Repositories\FunctionCodeRepository;
use APIs\Domain\Configurations\Functions\FunctionCode;
use APIs\Domain\Configurations\RawConfiguration;
use APIs\Domain\Https\Repositories\HttpQueryRepository;
use APIs\Domain\Https\URLs\URLAdapter;
use APIs\Infrastructure\Objects\RetrieverCriterias\ConcreteHttpQueryRetrieverCriteria;
use APIs\Domain\Https\Repositories\HttpQueryRetrieverCriteria;
use APIs\Infrastructure\Objects\ConcreteLanguageDefinition;
use APIs\Infrastructure\Objects\ConcreteFunctionCode;

final class ConcreteFunctionCodeRepository implements FunctionCodeRepository {

    private $httpQueryRepository;
    private $urlAdapter;
    public function __construct(HttpQueryRepository $httpQueryRepository, URLAdapter $urlAdapter) {
        $this->httpQueryRepository = $httpQueryRepository;
        $this->urlAdapter = $urlAdapter;
    }

    public function retrieveMultiple(RawConfiguration $configuration): Map<string, FunctionCode> {

        try {

            $functionCodes = Map<string, FunctionCode> {};
            $sourceCodes = $this->retrieveSourceCodes($configuration);
            foreach($sourceCodes as $keyname => $oneSourceCode) {

                $functionConfig = $configuration->get('functions->'.$keyname);
                $languageConfig = $configuration->get('languages->'.$functionConfig['language']);

                $languageHost = $this->urlAdapter->convertStringToURL($languageConfig['host']);
                $languageDefinition = new ConcreteLanguageDefinition($functionConfig['language'], $languageHost, $languageConfig['port']);
                $functionCodes[$keyname] = new ConcreteFunctionCode($functionConfig['name'], $languageDefinition, $oneSourceCode);
            }

            return $functionCodes;

        } catch (\Exception $exception) {
            throw $exception;
        }

    }

    private function retrieveSourceCodes(RawConfiguration $configuration): Map<string, string> {

        try {

            $criterias = Map<string, HttpQueryRetrieverCriteria> {};
            $functions = $configuration->get('functions');
            foreach($functions as $keyname => $oneFunction) {

                $httpMethod = 'get';
                $url = $this->urlAdapter->convertStringToURL($oneFunction['url']);
                $criterias[$keyname] = new ConcreteHttpQueryRetrieverCriteria($url, $httpMethod);
            }

            $output = Map<string, string> {};
            $sourceCodes = $this->httpQueryRepository->retrieve($criterias);
            foreach($sourceCodes as $keyname => $httpQuery) {

                $httpCode = $httpQuery->getHttpCode();
                if ($httpCode != 200) {
                    throw new \Exception('The function ('.$keyname.') returned an invalid http code ('.$httpCode.') when trying to retrieve its content.  Its url was: '.$httpQuery->getUrl());
                }

                $content = $httpQuery->getContent();
                if (trim($content) == '') {
                    throw new \Exception('The function ('.$keyname.') returned empty content.  Its url was: '.$httpQuery->getUrl());
                }

                $output[$keyname] = $content;

            }

            return $output;

        } catch (\Exception $exception) {
            throw $exception;
        }
    }

}
