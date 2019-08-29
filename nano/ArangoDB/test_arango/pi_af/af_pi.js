function af2pi(af_name){
	var res = db._query(
	    {
		    'query':"for af in org filter af.name== @af_name \
				        for per in 1..1 inbound af work \
				            for pap in 1..1 outbound per author_of \
				                for pi in 1..1 inbound pap author_of \
				                    for part in participation filter part._from==pi._id and  part.position=='PI' \
				                        RETURN distinct { 'path': CONCAT_SEPARATOR( ' -> ', af.name,pi.name ), 'through':per.name }",
		    'bindVars':{ 'af_name': af_name }
        }
    ).toArray()  //console.log(res.constructor==Array) : true

    res.forEach( function(e){
	    var idx
	    while((idx = res.findIndex(x => x['path']===e['path'] && x['through']!=e['through']))!=-1 ){
		    e['through']+=( ', '+res[idx]['through'] )
	        res.splice(idx,1)
	    }
    });
    print(res)
}

///////////////////////////////////////////////////////

function afa2pi(af_arr){
	var res = []
	af_arr.forEach( function(af_name){
		var new_pi = db._query(
	    {
		    'query':"for af in org filter af.name== @af_name \
				        for per in 1..1 inbound af work \
				            for pap in 1..1 outbound per author_of \
				                for pi in 1..1 inbound pap author_of \
				                    for part in participation filter part._from==pi._id and  part.position=='PI' \
				                        RETURN distinct { 'path': CONCAT_SEPARATOR( ' -> ', af.name,pi.name ), 'through':per.name }",
		    'bindVars':{ 'af_name': af_name }
        }).toArray()

        res = res.concat(new_pi);
	});

    res.forEach( function(e){
	    var idx
	    while((idx = res.findIndex(x => x['path']===e['path'] && x['through']!=e['through']))!=-1 ){
		    e['through']+=( ', '+res[idx]['through'] )
	        res.splice(idx,1)
	    }
    });
    return res
    //print(res)
}