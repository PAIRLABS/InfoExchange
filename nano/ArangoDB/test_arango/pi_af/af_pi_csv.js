db._useDatabase('test')

const fs = require('fs') 

var af_arr = new Set();

var pi_arr = db._query('for per in person \
                            for part in participation filter part._from==per._id and part.position=="PI"\
                                return per.name ').toArray();
var res_af = sort_pi_af(pia2af(pi_arr))


res_af.forEach( function(e){
    af_arr.add(e['af'])
} )

fs.writeFileSync('pi_to_af.csv',arr2str({data : res_af, ignore_same : 'pi'}) , (err) => { 
    if (err) throw err; 
}) 

var res_pi = sort_af_pi(afa2pi(Array.from(af_arr)))


fs.writeFileSync('af_to_pi.csv', arr2str({data : res_pi, ignore_same : 'af'}), (err) => { 
    if (err) throw err; 
}) 


///////////////////////////////////////////////////////////////////////

function isChinese(temp){
    var re=/[^\u4e00-\u9fa5]/;
    if(re.test(temp)) return false;
    return true;
}

function sort_pi_af(obj){
    obj.sort(
       function(a, b) {          
          if (a.pi === b.pi) {
             return a.af > b.af ? 1:-1
          }
          return a.pi > b.pi ? 1 : -1;
       });
    return obj
}

function sort_af_pi(obj){
    obj.sort(
       function(a, b) {          
          if (a.af === b.af) {
             return a.pi > b.pi ? 1:-1
          }
          return a.af > b.af ? 1 : -1;
       });
    return obj
}


function arr2str(args) {    //https://halistechnology.com/2015/05/28/use-javascript-to-export-your-data-as-csv/
        var result, ctr, keys, columnDelimiter, lineDelimiter, data, temp, ignore_same;

        data = args.data || null;
        if (data == null || !data.length) {
            return null;
        }

        columnDelimiter = args.columnDelimiter || ',';
        lineDelimiter = args.lineDelimiter || '\n';
        ignore_same = args.ignore_same || '';

        keys = Object.keys(data[0]);

        result = '';
        result += keys.join(columnDelimiter);
        result += lineDelimiter;

        temp='';
        data.forEach(function(item) {
            if(temp==item[ignore_same] && ignore_same!='')
                item[ignore_same]=''
            else
                temp=item[ignore_same]
                

            ctr = 0;
            keys.forEach(function(key) {
                if (ctr > 0) result += columnDelimiter;

                result += item[key];
                ctr++;
            });
            result += lineDelimiter;
        });
        //print(result)
        return result;
    }

function pia2af(pi_arr){
	var res =[]
	pi_arr.forEach( function(pi_name){
		var new_af = db._query(
	    {
            'query':" let ex_per = (for per in person for part in participation filter part._from==per._id return per._id)\
                    for pi in person filter pi.name==@pi_name  \
                        for pap in 1..1 outbound pi author_of  \
                            for co_author in 1..1 inbound pap author_of  \
                                for af in 1..1 outbound co_author work  \
                                    filter co_author._id not in ex_per \
                                    RETURN distinct { 'pi':pi.name, 'af':af.name, 'through':co_author.name}",
            'bindVars':{ 'pi_name': pi_name }
        }).toArray()  //console.log(res.constructor==Array) : true

        res = res.concat(new_af);
	});

    return clear_path(res)
}

function afa2pi(af_arr){
	var res = []
	af_arr.forEach( function(af_name){
		var new_pi = db._query(
	    {
		    'query':" let ex_per = (for per in person for part in participation filter part._from==per._id return per._id)\
                    for af in org filter af.name== @af_name \
				        for co_author in 1..1 inbound af work \
				            for pap in 1..1 outbound co_author author_of \
				                for pi in 1..1 inbound pap author_of \
				                    filter co_author._id not in ex_per \
                                        for part in participation filter part._from==pi._id and part.position=='PI' \
				                        RETURN distinct { 'af':af.name, 'pi':pi.name , 'through':co_author.name }",
		    'bindVars':{ 'af_name': af_name }
        }).toArray()

        res = res.concat(new_pi);
	});
    return clear_path(res)
}

function clear_path(res){
    var idx
    while((idx=res.findIndex(x => isChinese(x['af'])))!=-1){
        res.splice(idx,1)
    }

    res.forEach( function(e){
        var idx
        var temp = new Set()
        temp.add(e['through'])
        while((idx = res.findIndex(x => x['pi']===e['pi'] && x['af']===e['af'] && x['through']!=e['through'] ))!=-1 ){
            if(res[idx]['through'].replace(/\s+/g,'')!=e['through'].replace(/\s+/g,'')) temp.add(res[idx]['through'])
            res.splice(idx,1)
        }
        e['through'] = Array.from(temp).sort().join(',')
    });
    return res
}
